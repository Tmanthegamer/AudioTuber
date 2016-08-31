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
#include "boost/filesystem.hpp"
#include <fstream>
#include <sstream>

typedef struct DownloadInfo
{
    long long       pos = 0;
    long long       max = 0;
    bool            active = false;
    char*           songdata;
} DownloadInfo;

class Song
{
public: // Public methods

    /* Constructors */
    Song(std::string name, std::string fp, std::string ext); // Primary constructor for finished songs.

    Song(std::string url);  // Constructor for songs being downloaded.

    Song(const Song& song); // copy constructor

    Song();
    
    ~Song();

    /* Move Assignment */
    Song& operator= (Song &&);

    /* Operators */
    bool operator==(const Song& song) const;
    Song& operator=(const Song& song);

    /* Getters */
    std::string getSongName() const;
    std::string getSongName();

    std::string getSongFilePath() const;
    std::string getSongFilePath();

    std::string getSongUrl() const;
    std::string getSongUrl();

    std::string getSongExt() const;
    std::string getSongExt();

    std::string getJson() const;
    std::string getJson();

    long long getSize() const;
    long long getSize();

    DownloadInfo getDownloadInfo() const;
    DownloadInfo getDownloadInfo();

    const bool& isExists();

    bool getExists() const;
    bool getExists();

    /* Setters */
    void setSongName(const std::string& name);

    void setSongFilePath(const std::string& name);

    void setSongUrl(const std::string& name);

    void setSongExists(const std::string& name, const std::string& fp, const std::string& ext);
    void setSongExists(const bool& exists = false);

    void setSongExt(const std::string& ext);

    std::string getInitialPacket();

    bool fillPacket(const long& max, long* filled);

    char* getPacket();
    
private: // Private functions
    bool setSongMaxSize(const long long& size = -1);

    void resetDownloadPosition();

    void setDownloadPosition(const long long& pos = 0LL);

    size_t fillPacketWithData(char* data, const long& max);

    bool downloadInit(const size_t& packet_size);

    void downloadCleanup();

    template <class T>
    std::string CreateJsonKeyValue(const std::string& key, const T& value, bool comma = false);

public: // Public variables
    static int SongsInQueue;

private:
    std::string   _name;
    std::string   _fp;
    std::string   _url;
    std::string   _ext;
    std::string   _json;
    bool          _inSystem;
    long long     _maxsize;   // Maximum file size
    DownloadInfo  _dl;

};

#endif //SONG_H
