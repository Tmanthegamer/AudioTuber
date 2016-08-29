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
    Song(std::string name, std::string fp, std::string ext);

    Song(std::string url);

    Song() = default;
    
    ~Song();

    /* Operators */
    bool operator==(const Song& song) const;

    /* Getters */
    std::string getSongName() const;

    std::string getSongFilePath() const;

    std::string getSongUrl() const;

    std::string getSongExt() const;

    const bool& isExists();

    /* Setters */
    void setSongName(const std::string& name);

    void setSongFilePath(const std::string& name);

    void setSongUrl(const std::string& name);

    void setSongExists(const std::string& name, const std::string& fp, const std::string& ext);

    void setSongExt(const std::string& ext);
    
public: // Public variables
    static int SongsInQueue;

private:
    std::string _name;
    std::string _fp;
    std::string _url;
    std::string _ext;
    std::string _json;
    bool        _inSystem;

};

#endif //SONG_H
