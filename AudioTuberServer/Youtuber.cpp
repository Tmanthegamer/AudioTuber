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
    std::string unwanted("info.json");

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

    std::string name = FindSongName(url);

    //s.setSongExists(name, fp, ext);
    //_ready.push_back(s);
    //_queue.erase(it);


    return true;
}

std::string const Youtuber::FindSongName(const char* url)
{
    std::string path(TEMP);
    path += "/newsongs.txt";

    std::ifstream file(path);
    if (!file)
    {
        std::cerr << "Could not open file " << path << std::endl;
        return std::string();
    }

    std::string line;
    std::string url_regex("[-a-zA-Z0-9@:\%._\\+~#=]{2,256}\\.[a-z]{2,6}\b([-a-zA-Z0-9@:\%_\\+.~#?&//=]*)");
    std::regex base_regex("\"(" + url_regex + ")\":\"(.*)\"");
    std::smatch pieces;

    while (getline(file, line))
    {
        if(std::regex_match(line, pieces, base_regex))
        {
            std::cout << "Size:" << pieces.size() << std::endl;
            for(size_t i = 0; i < pieces.size(); ++i)
            {
                std::cout << i << "[" << pieces[i].str() << "]" << std::endl;
            }
        }      
    }
}

bool Youtuber::SetServer(const Server& svr)
{
    _svr = svr;
}