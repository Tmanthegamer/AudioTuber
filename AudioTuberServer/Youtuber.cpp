#include "boost/filesystem.hpp"
#include "Youtuber.h"
#define BOOST_FILESYSTEM_NO_DEPRECATED

namespace fs = boost::filesystem;

Youtuber::Youtuber()
    : _svr()
{
    if(InitializeSongReadyList())
    {
        GetReadySongList();
    }
}

int Youtuber::RunYoutubeDL(char *url)
{
    (void)url;
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
    std::string path("/home/tyler/AudioTuber/AudioTuberServer/storage");
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

const std::vector<std::string> Youtuber::GetReadySongList()
{
    std::vector<std::string> songlist;
    for (std::vector<Song>::iterator it = _ready.begin() ; it != _ready.end(); ++it)
    {
        songlist.push_back(it->getSongName());
    }
    
    return songlist;  
}

bool Youtuber::SetServer(const Server& svr)
{
    _svr = svr;
}