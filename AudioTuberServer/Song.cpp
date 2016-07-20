#include "Song.h"

int Song::SongsInQueue = 0;

bool Song::operator==(const Song& song) const
{
    if(getSongFilePath().empty() && song.getSongFilePath().empty())
    {
        return getSongUrl() == song.getSongUrl();
    }
    
    return getSongFilePath() == song.getSongFilePath();
}

Song::Song(std::string name, std::string fp, std::string ext)
    :_name(name),
     _fp(fp),
     _ext(ext),
     _url(std::string("")), 
     _inSystem(true)
{
    _json = name.append(".info.json");
}

Song::Song(std::string url)
    :_name(std::string("")), 
     _json(std::string("")),
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

std::string Song::getSongName() const
{
    return _name;
}

std::string Song::getSongUrl() const
{
    return _url;
}

std::string Song::getSongFilePath() const
{
    return _fp;
}

std::string Song::getSongExt() const
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

void Song::setSongExists(const std::string name, const std::string fp, const std::string ext)
{
    _name = name;
    _ext = ext;
    _fp = fp;
    _url.clear();
    _inSystem = true;
}

void Song::setSongExt(const std::string ext)
{
    _ext = ext;
}