int Song::SongsInQueue = 0;

Song(std::string name, std::string fp)
    :_name(name), 
     _fp(fp), 
     _url(std::string("")), 
     _inSystem(true)
{

}

Song(std::string url)
    :_name(std::string("")), 
     _fp(std::string("")), 
     _url(url), 
     _inSystem(false)
{
    SongsInQueue++;
}

Song::~Song()
{
    if(!_inSystem)
    {
        SongsInQueue--;
    }
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

void Song::setSongName(std::string name)
{
    _name = name;
}

void Song::setSongFilePath(std::string fp)
{
    _fp = fp;
}

void Song::setSongUrl(std::string url)
{
    _url = url;
}

void Song::setSongExists(bool exists)
{
    _inSystem = exists;
}