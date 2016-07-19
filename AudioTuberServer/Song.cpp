#include "Song.h"

int Song::SongsInQueue = 0;

Song::Song(std::string name, std::string fp, std::string ext)
    :_name(name), 
     _fp(fp),
     _ext(ext),
     _url(std::string("")), 
     _inSystem(true)
{

}

Song::Song(std::string url)
    :_name(std::string("")), 
     _fp(std::string("")), 
     _ext(std::string("")),
     _url(url), 
     _inSystem(false)
{
    SongsInQueue++;
}

Song::~Song()
{
    if(!_inSystem) SongsInQueue--;
}

std::string Song::getSongName()
{
    return _name;
}

std::string Song::getSongUrl()
{
    return _url;
}

std::string Song::getSongFilePath()
{
    return _fp;
}

std::string Song::getSongExt()
{
    return _ext;
}

void Song::setSongName(const std::string name)
{
    _name = name;
}

void Song::setSongFilePath(const std::string fp)
{
    _fp = fp;
}

void Song::setSongUrl(const std::string url)
{
    _url = url;
}

void Song::setSongExists(const bool exists)
{
    _inSystem = exists;
}

void Song::setSongExt(const std::string ext)
{
    _ext = ext;
}