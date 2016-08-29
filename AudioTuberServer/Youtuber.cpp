#include "boost/filesystem.hpp"
#include "Youtuber.h"
#define BOOST_FILESYSTEM_NO_DEPRECATED

namespace fs = boost::filesystem;

void Download(const char* url)
{
    int fd = open("/dev/null", O_WRONLY);
    if(fd < 0)
        perror("Download: redirect");
    else
    {
        dup2(fd, 1);  // redirect stdout
        //dup2(fd, 2);  // redirect stdout
    }

    std::vector<std::string> argv;
    argv.push_back(std::string("/bin/bash"));
    argv.push_back(std::string("bash"));
    argv.push_back(std::string(SCRIPT));
    argv.push_back(std::string(url));

    std::cout << "Starting execl" << std::endl;

    if(execl(argv[0].c_str(), 
        argv[1].c_str(), 
        argv[2].c_str(), 
        argv[3].c_str(),        
        (char *)0))
    {
        perror("Download execl");
    }

    // THE CHILD WILL NEVER REACH HERE, IT IS REPLACED ENTIRELY
}

Youtuber::Youtuber()
    : _svr(), active(false)
{
    if(!InitializePaths())
    {
        // Error handling here
    }

    if(!InitializeSongReadyList())
    {
        // Error handling here
    }

}

Youtuber::~Youtuber()
{
    _ready.clear();
    _queue.clear();
}

int Youtuber::RunYoutubeDL(const char *url)
{
    std::cout << "Creating child process..." << std::endl;
    _queue.push_back(Song(std::string(url)));

    while(active)
    {
        std::cerr << "Sleep 5" << std::endl;
        sleep(10);
        active = false;
    }

    active = true;
    active_song = new Song(std::string(url));

    pid_t child = fork();
    if (child == 0)
    {
        Download(url);
        return 0;
    } 
    else if(child == -1)
    {
        std::cout << "Could not download..." << std::endl;
    }

    std::cerr << "VerifyDownload" << std::endl;
    if(!VerifyDownload(url)) 
        return 0; 

    return 1;
}

void Youtuber::AddSong(char* url)
{
    (void)url;
}

int Youtuber::GetFileSize(char* file)
{
    (void)file;
    return 1;
}

char* Youtuber::PrepareNextPacket()
{
    return (char*)0;
}

int Youtuber::UploadSong(char* song)
{
    (void)song;
    return 1; 
}

//Refreshes the song list by peeking at all the files loaded in Storage
bool Youtuber::InitializeSongReadyList()
{
    std::string path(STORAGE);
    std::string unwanted(".json");

    if (!path.empty())
    {
        namespace fs = boost::filesystem;

        fs::path apk_path(path);
        fs::recursive_directory_iterator end;

        for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
        {
            const fs::path cp = (*i);

            std::string ext = boost::filesystem::extension(cp);

            // Ignore json files
            if(ext.compare(unwanted) == 0) { continue; }

            std::string name = boost::filesystem::basename(cp);
            std::string fp = cp.string();
            
            Song s(name, fp, ext);
            _ready.push_back(Song(name, fp, ext));
        }
    }

    return _ready.size() > 0;
}

bool Youtuber::InitializePaths()
{
    /* 
    TODO: initialize >>
    std::string _storagepath;   
    std::string _scriptpath;
    std::string _serverPath
    */
}

std::vector<std::string> Youtuber::GetReadySongList()
{
    std::vector<std::string> songlist;
    for (std::vector<Song>::iterator it = _ready.begin() ; it != _ready.end(); ++it)
    {
        songlist.push_back(it->getSongName());
    }
    
    return songlist;  
}

void Youtuber::CheckSongFinished(const char* url)
{
    std::string path(TEMP);
    std::string unwanted(".json");

    if (!path.empty())
    {
        namespace fs = boost::filesystem;

        fs::path apk_path(path);
        fs::recursive_directory_iterator end;

        for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
        {
            const fs::path cp = (*i);

            std::string ext = boost::filesystem::extension(cp);

            // Ignore json files
            if(ext.compare(unwanted) == 0) { continue; }

            std::string name = boost::filesystem::basename(cp);
            std::string fp = cp.string();
            
            Song s(name, fp, ext);
            _ready.push_back(Song(name, fp, ext));
        }
    }
}

std::vector<std::string> Youtuber::GetAllNewJson()
{
    std::string path(TEMP);
    //std::string path(STORAGE);
    std::string wanted(".json");
    std::vector<std::string> jsonlist;

    if (!path.empty())
    {
        namespace fs = boost::filesystem;
        
        if(!(boost::filesystem::exists(path)))
        {
            boost::filesystem::create_directory(path);
        }

        fs::path apk_path(path);
        fs::recursive_directory_iterator end;

        for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
        {
            const fs::path cp = (*i);
            std::string ext = boost::filesystem::extension(cp);

            // Ignore all non-json files
            if(ext.compare(wanted) != 0) { continue; }

            jsonlist.push_back(cp.string());
        }
    }
    return jsonlist;
}

std::vector<Song>::iterator Youtuber::FindQueuedSong(const char* url)
{
    Song s(url);
    return std::find(_queue.begin(), _queue.end(), s);
}

bool Youtuber::VerifyDownload(const char* url)
{
    std::vector<Song>::iterator it = FindQueuedSong(url);
    if(it == _queue.end()){
        std::cerr << "Not found" << std::endl;
        return false;
    }

    Song s = FindDownloadedSongFromURL(url);
    if(!s.isExists())
    {
        return false;
    }

    changeFilePathString(s);

    it->setSongExists(s.getSongName(), s.getSongFilePath(), s.getSongExt());
    std::cerr << "path:" << it->getSongFilePath() << std::endl;
    _ready.push_back(*it);
    _queue.erase(it);

    //for(auto& x : _ready) { std::cerr << "Ready:" << x.getSongName() << std::endl; }
    //for(auto& x : _queue) { std::cerr << "Queue:" << x.getSongName() << std::endl; }
    
    return true;
}

Song Youtuber::FindDownloadedSongFromURL(const char* url)
{
    std::vector<std::string> jsonlist;
    int timer = 0;

    // TODO: check the time to see if more than 3min has past, if so stop this loop
    while(timer < 50)
    {
        jsonlist = GetAllNewJson();
        for(const auto& s : jsonlist)
        {
            std::string("");
            std::ifstream file;
            std::string line;
            file.open(s, std::ifstream::in);

            if (!file.good() || !file.is_open())
            {
                continue;
            }

            std::cout << "Working with: " << s << "..." << std::endl;
            while(file.good() && getline(file, line))
            {
                Song s = ParseSongFromJson(line, url);

                std::cerr << s.getSongFilePath() << std::endl;
                if(s.isExists() && boost::filesystem::exists(s.getSongFilePath()) ) 
                {
                    return s;
                }
            }
        }

        timer += 5;
        sleep(5);
    }
    
    return Song();
}

Song Youtuber::ParseSongFromJson(std::string line, std::string url)
{
    Song s;

    // Raw Strings
    std::string raw_url = R"(\"webpage_url\": \"(.*?)\")";
    std::string raw_name = R"(\"fulltitle\": \"(.*?)\")";
    std::string raw_ext = R"(\"ext\": \"(.*?)\")";
    std::string raw_filename = R"(\"_filename\": \"(.*?)\")";
    
    // Regex with the Raw Strings
    std::regex r_url(raw_url);
    std::regex r_name(raw_name);
    std::regex r_ext(raw_ext);
    std::regex r_file(raw_filename);

    // Smatch
    std::smatch url_match, name_match, ext_match, file_match;

    if(std::regex_search(line, url_match, r_url))
    {
        
        #if 0
        std::cout << "Matches..." << std::endl;
        for (auto x:url_match) std::cout << x << "||";
        #endif
        
        // Check to see if this is the song we want to parse.
        if(url.compare(url_match.str(1)) == 0)
        {
            std::string name, fp, ext;

            // Extract the name of the song.
            if(std::regex_search(line, name_match, r_name))
            {
                name = name_match.str(1);
            }

            // Extract the ext of the song.
            if(std::regex_search(line, ext_match, r_ext))
            {
                ext = ext_match.str(1);
            }

            // Extract the file path of the song.
            if(std::regex_search(line, file_match, r_file))
            {
                fp = file_match.str(1);
            }

            // If all the requirements were filled, return a Song.
            if(name.length() > 0 && ext.length() > 0 & fp.length() > 0)
            {
                s = Song(name, fp, ext);
            }

        }
    }

    return s;
}

bool Youtuber::SetServer(const Server& svr)
{
    _svr = svr;
}

Song Youtuber::FindReadySongByName(const std::string name)
{
    for(Song& s : _ready)
    {

    }
}

void Youtuber::changeFilePath(Song& song)
{
    std::string filepath = song.getSongFilePath();
    
    if(boost::filesystem::exists(filepath))
    {
        const std::string temppath(TEMP);
        const std::string storagepath(STORAGE);

        size_t start_pos = filepath.find(TEMP);
        if(start_pos == std::string::npos)
            return;

        std::string newfilepath = storagepath + std::string(filepath.begin() + temppath.size(), filepath.end());

        boost::filesystem::rename(filepath, newfilepath);
        boost::filesystem::remove(filepath);

        song.setSongFilePath(filepath);
    }
    
}