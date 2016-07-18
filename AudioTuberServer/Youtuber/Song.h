#ifndef SONG_H
#define SONG_H

/*-------------------------------------------------------------------------------------
--  SOURCE FILE:    Song.h - Youtube-dl manager
--
--  PROGRAM:        svr.out
--
--  FUNCTIONS:      
--
--  DATE:           July 18, 2016
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

#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        // For mode constants

class Song
{
public: // Public methods
    Song(std::string name, std::string fp);

    Song(std::string url);
    
    ~Song(){}

    std::string getSongName();

    std::string getSongFilePath();

    std::string getSongUrl();

    const bool& isExists();

    void setSongName(std::string name);

    void setSongFilePath(std::string name);

    void setSongUrl(std::string name);

    void setSongExists(bool exists);
    
public: // Public variables
    static int SongsInQueue;

private:
    std::string _name;
    std::string _fp;
    std::string _url;
    bool        _inSystem;

};

#endif //SONG_H
