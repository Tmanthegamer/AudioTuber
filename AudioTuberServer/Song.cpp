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

Song& Song::operator=(const Song& song)
{
    if(this != &song)
    {
        _name = song.getSongName();
        _fp = song.getSongFilePath();
        _url = song.getSongUrl();
        _ext = song.getSongExt();
        _json = song.getJson();
        _inSystem = song.getExists();
        _maxsize = song.getSize();
        _dl = song.getDownloadInfo();
    }

    return *this;
}

Song& Song::operator= (Song && song)
{
    if(this != &song)
    {
        _name = song.getSongName();
        _fp = song.getSongFilePath();
        _url = song.getSongUrl();
        _ext = song.getSongExt();
        _json = song.getJson();
        _inSystem = song.getExists();
        _maxsize = song.getSize();
        _dl = song.getDownloadInfo();

        song.setSongName("");
        song.resetDownloadPosition();
        song.setSongUrl("");
        song.setSongMaxSize();
        song.setSongFilePath("");
        song.setSongExt("");
        song.setSongExists(false);
    }

    return *this;
}

Song::Song(std::string name, std::string fp, std::string ext)
    :_name(name),
     _fp(fp),
     _ext(ext),
     _url(std::string("")), 
     _inSystem(true),
     _maxsize(0)
{
    _json = name.append(".info.json");
    setSongMaxSize();
}

Song::Song(std::string url)
    :_name(std::string("")), 
     _json(std::string("")),
     _fp(std::string("")), 
     _ext(std::string("")),
     _url(url), 
     _inSystem(false),
     _maxsize(0)
{
    SongsInQueue++;
}

Song::Song()
{

}

Song::Song(const Song& song)
{
    *this = song;
}

Song::~Song()
{
    if(!_inSystem) SongsInQueue--;
    
    downloadCleanup();
}

std::string Song::getSongName() const
{
    return _name;
}

std::string Song::getSongName()
{
    return _name;
}

std::string Song::getSongUrl() const
{
    return _url;
}

std::string Song::getSongUrl()
{
    return _url;
}

std::string Song::getSongFilePath() const
{
    return _fp;
}

std::string Song::getSongFilePath()
{
    std::string copy(_fp);
    return copy;
}

std::string Song::getSongExt() const
{
    return _ext;
}

std::string Song::getSongExt()
{
    return _ext;
}

std::string Song::getJson() const
{
    return _json;
}

std::string Song::getJson()
{
    return _json;
}

long long Song::getSize() const
{
    return _maxsize;
}

long long Song::getSize()
{
    return _maxsize;
}

DownloadInfo Song::getDownloadInfo() const
{
    return _dl;
}

DownloadInfo Song::getDownloadInfo()
{
    return _dl;
}

const bool& Song::isExists()
{
    return _inSystem;
}

bool Song::getExists() const
{
    return _inSystem;
}

bool Song::getExists()
{
    return _inSystem;
}

void Song::setSongName(const std::string& name)
{
    _name = name;
}

void Song::setSongFilePath(const std::string& fp)
{
    _fp = fp;
}

void Song::setSongUrl(const std::string& url)
{
    _url = url;
}

void Song::setSongExists(const std::string& name, const std::string& fp, const std::string& ext)
{
    _name = name;
    _ext = ext;
    _fp = fp;
    _url.clear();
    _inSystem = true;

    setSongMaxSize();
}

void Song::setSongExists(const bool& exists)
{
    _inSystem = exists;
}

void Song::setSongExt(const std::string& ext)
{
    _ext = ext;
}

bool Song::setSongMaxSize(const long long& size)
{
    if(!(boost::filesystem::exists(_fp)))
    {
        return false;
    }
    
    if(size == -1) // Default argument provided
    {   
        std::ifstream in(_fp.c_str(), std::ifstream::ate | std::ifstream::binary);
        const long long temp = in.tellg(); 
        in.close();
        if(temp < 0)
            return false;

        _maxsize = temp;

    }
    else // Argument provided
    {
        if(size <= 0)
            return false;

        _maxsize = size;

    }

    return true;
}

void Song::resetDownloadPosition()
{
    setDownloadPosition();
}

void Song::setDownloadPosition(const long long& pos)
{
    if(pos >= 0)      
    {
        

        _dl.pos = pos;
    } 
}

// Max is the maximum packet size
size_t Song::fillPacketWithData(char* data, const long& max)
{
    long filled = 0;

    if(_dl.pos >= _dl.max)
    {
        return 0;
    }

    // Check to see if we need to clear the data before filling it when sending it.
    if(_dl.max - _dl.pos < (long long) max)
    {
        long adjust = _dl.max - _dl.pos;
        memset(data + adjust, 0, max-adjust);
    }

    // Seek to the song position and read in the maximum amount of bytes.
    std::ifstream in(_fp.c_str(), std::ifstream::ate | std::ifstream::binary);
    in.seekg (_dl.pos, in.beg);

    in.read (data, max);
    in.close();

    // Get the amount of bytes read and add it to the position if successful.
    filled = in.gcount();
    if(filled > 0)
    {
        _dl.pos += filled;
    }
    else
    {
        filled = 0;
    }

    return filled;
}

std::string Song::getInitialPacket()
{
    std::ostringstream oss; // Header Packet container

    long long max_size;
    long long packet_size;
    long long packet_num;
    long long allocated;
    long long last_size;
    short last_packet;

    std::string filename = _name;
    std::string ext = _ext;

    max_size = _maxsize;
    packet_size = 1024;
    packet_num = max_size / packet_size;

    allocated = packet_num * packet_size;
    
    if(allocated < max_size)
    {
        last_packet = 1;
        last_size = max_size - allocated;
        packet_num++;
    }
    else
    {
        last_packet = 0;
        allocated = 0;
    }

    oss << "{" << CreateJsonKeyValue("size", max_size, true);
    oss << CreateJsonKeyValue("packets", packet_num, true);
    oss << CreateJsonKeyValue("packet_size", packet_size, true);
    oss << CreateJsonKeyValue("filename", filename, true);
    oss << CreateJsonKeyValue("ext", ext, true);
    oss << CreateJsonKeyValue("last_packet", last_packet, true);
    oss << CreateJsonKeyValue("last_size", last_size);
    oss << "}";

    return oss.str();
}

template <class T>
std::string Song::CreateJsonKeyValue(const std::string& key, const T& value, bool comma)
{
    std::ostringstream oss;
    oss << "\"" << key << "\":\"" << value << "\"";
    
    if(comma)
    {
        oss << ",";
        
    }

    return oss.str();
}

bool Song::downloadInit(const size_t& packet_size)
{
    if(packet_size == 0 || _dl.active || !setSongMaxSize()) 
    { 
        return false; 
    }

    // Make room for the data
    _dl.songdata = (char*) malloc(sizeof(char) * (packet_size+1));
    memset(_dl.songdata, 0, packet_size+1);
    
    if(_dl.songdata == NULL) 
    { 
        return false; 
    }

    _dl.max = _maxsize;
    _dl.pos = 0LL;
    _dl.active = true;

    return true;
}

void Song::downloadCleanup()
{
    if(_dl.active)
    {
        free(_dl.songdata);

        _dl.max = 0LL;
        _dl.pos = 0LL;
        _dl.active = false;
    }

}

char* Song::getPacket()
{
    return _dl.songdata;
}

bool Song::fillPacket(const long& max, long* filled)
{
    if(!_dl.active)
    {
        if(!downloadInit(1024)) { return false; }
    }

    (*filled) = fillPacketWithData(_dl.songdata, max);

    if((*filled) == 0)
    {
        return false;
    }

    return true;
}
