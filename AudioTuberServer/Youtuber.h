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
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        // For mode constants
#include "Server.h"

class Youtuber
{
public:
    Youtuber();
    ~Youtuber(){}

    int RunYoutubeDL(char *url);

    void AddSong(char* url);

    int GetFileSize(char* file);

    char* PrepareNextPacket();

    int UploadSong(char* song);

    std::vector<std::string> GrabAllSongs();
    

private:
    Server svr;
    std::vector<std::string> _queue;    // Queue for songs to be downloaded
    std::vector<std::string> _ready;    // Queue for songs that are ready
};

#endif //YOUTUBER_H
