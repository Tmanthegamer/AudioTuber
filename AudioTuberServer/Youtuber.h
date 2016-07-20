#ifndef YOUTUBER_H
#define YOUTUBER_H

/*-------------------------------------------------------------------------------------
--  SOURCE FILE:    Youtuber.h - Youtube-dl manager
--
--  PROGRAM:        svr.out
--
--  FUNCTIONS:      
--
--  DATE:           July 11, 2016
--
--  REVISIONS:      
--
--  DESIGNERS:      Tyler Trepanier
--
--  PROGRAMMERS:    Tyler Trepanier
--
--  NOTES:
--  
-------------------------------------------------------------------------------------*/

#include <fcntl.h>           // For O_* constants
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        // For mode constants
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <boost/thread.hpp>
#include "boost/progress.hpp"
#include "Server.h"
#include "Song.h"

#define STORAGE     "/home/tyler/AudioTuber/AudioTuberServer/storage"
#define SERVER      "/home/tyler/AudioTuber/AudioTuberServer/"
#define SCRIPT      "/home/tyler/AudioTuber/AudioTuberServer/music.sh"
#define TEMP        "/home/tyler/AudioTuber/AudioTuberServer/temp"

class Youtuber
{
public:
    Youtuber();
    
    ~Youtuber();

    int RunYoutubeDL(const char *url);

    void AddSong(char* url);

    int GetFileSize(char* file);

    std::vector<std::string> GetReadySongList();

    char* PrepareNextPacket();

    int UploadSong(char* song);

    void CheckSongFinished(const char* url);

    bool VerifyDownload(const char* url);

    std::vector<Song>::iterator FindQueuedSong(const char* url);

    std::string const FindSongName(const char* url);

    bool InitializePaths();

    bool InitializeSongReadyList();

    bool SetServer(const Server& svr);
    
private: // Private Methods


private:
    Server      _svr;
    Song        *active_song;
    bool        active;
    std::string _temppath;      // File system temp storage path
    std::string _storagepath;   // File system storage path
    std::string _scriptpath;    // File system script path
    std::string _serverPath;    // File system server directory path
    std::vector<Song> _queue;   // Queue for songs to be downloaded
    std::vector<Song> _ready;   // Queue for songs that are ready
    
};

void Download(const char* url);

#endif //YOUTUBER_H
