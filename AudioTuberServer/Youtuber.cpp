#include "boost/filesystem.hpp"
#include "Youtuber.h"
#define BOOST_FILESYSTEM_NO_DEPRECATED

namespace fs = boost::filesystem;

void Download(char* url)
{
    std::cout << "Starting download..." << std::endl;

    std::vector<std::string> argv;
    argv.push_back(std::string("/bin/sh"));
    argv.push_back(std::string("sh"));
    argv.push_back(std::string(SCRIPT));
    argv.push_back(std::string(url));

    if(execl(argv[0].c_str(), argv[1].c_str(), argv[2].c_str(), argv[3].c_str(), (char *)0))
    {
        perror("execl");
    }

    // THE CHILD WILL NEVER REACH HERE, IT IS REPLACED ENTIRELY
}

Youtuber::Youtuber()
    : _svr(), _active(false)
{
    if(!InitializePaths())
    {
        // Error handling here
    }

    if(!InitializeSongReadyList())
    {
        // Error handling here
    }

    RunYoutubeDL("https://www.youtube.com/watch?v=ZSzoKL-iO5M");
}

~Youtuber()
{
    _ready.clear();
    _queue.clear();

    std::cout << "sizes:" << _ready.size() << "][" << _queue.size() << std::endl;
}

int Youtuber::RunYoutubeDL(char *url)
{
    active_song = new Song(std::string(url));
    active = true;

    std::cout << "Starting download...\n" << std::endl;

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

    std::cout << "\nDownload Started..." << std::endl;
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
    return "";
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
            _ready.push_back(new Song(name, fp, ext));
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

const std::vector<std::string> Youtuber::GetReadySongList()
{
    std::vector<std::string> songlist;
    for (std::vector<Song>::iterator it = _ready.begin() ; it != _ready.end(); ++it)
    {
        songlist.push_back(it->getSongName());
    }
    
    return songlist;  
}

void Youtuber::CheckSongFinished()
{
    
}

bool Youtuber::SetServer(const Server& svr)
{
    _svr = svr;
}