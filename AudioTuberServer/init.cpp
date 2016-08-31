#include "Youtuber.h"

/*-------------------------------------------------------------------------------------
--  FUNCTION:       Main
--
--  DATE:           Feb 29, 2016
--
--  REVISED:        March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.                    
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int main (int argc, char **argv)
--
--  PARAMETERS:     int argc
--                      Number of arguments run from command line
--                  char **argv
--                      Arguments included from command line
--
--  RETURNS:        int
--                      -Returns 0 when there is no error with Program execution
--                      -Returns BADHOST (10) when the client was unable to 
--                          connect to the host indicated from comand line.
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          a TCP socket
--                      -Returns BUFFEROVERFLOW (30) when a message received a
--                          bufferflow error
--                      -Returns SOCKOPTERROR (50) when the server was unable to
--                          set the socket operation. Required for the select
--                          function.
--
--  NOTES:
--  Main entry into the program. Simply creates an instance of a server, initializes 
--  the server and begins the server program.
-------------------------------------------------------------------------------------*/
int main (int argc, char** argv)
{
    Server *svr = new Server();
    Youtuber* tube = new Youtuber();
    
    #if 1
    switch(argc)
    {
        case 4:
        {
            Song song(argv[1], argv[2], argv[3]);
            std::cerr << song.getInitialPacket() << std::endl;
            break;
        }
        default:
            std::cerr << "That's not how you do it." << std::endl;
            break;
    }
    #endif

    #if 0
    // Run Youtube DL (cmd-line youtube argument), dl, parse, move to storage
    // WORKSSSSSS
    switch(argc)
    {
        case 2:
        {
            std::cout << "argv[1] is:" << argv[1] << std::endl;
            std::cout << tube->RunYoutubeDL(argv[1]) << std::endl;
        }
            break;
        default: //argc > 1
            std::cout << "Argc is not 2" << std::endl;
            break;
    }
    #endif

    #if 0 
    //FindSongName tests
    switch(argc)
    {
        case 2:
        {
            std::cout << "argv[1] is:" << argv[1] << std::endl;
            std::cout << tube->FindSongName(argv[1]) << std::endl;
        }
            break;
        default: //argc > 1
            std::cout << "Argc is not 2" << std::endl;
            break;
    }
    #endif

    #if 0
    //In progress, working on getting song name from file
    switch(argc)
    {
        case 1:
            break;
        default: //argc > 1
        {
            std::vector<std::string> v;
            for(int i = 1; i < argc; i++)
            {
                std::cout << "Doing:" << argv[i] << std::endl;
                tube->RunYoutubeDL(argv[i]);
            }
        }
    }
    delete tube;

    #endif

    #if 0 // Used for testing purposes
    //char* data = (char*) malloc(PACKET_SIZE);
    int result;
    char data[256] = "[0][https://www.youtube.com/watch?v=_wjpQG9e9xA&list=RD_GRW6N]";
    if((result = svr->ParseHeaderData(data)) != SUCCESS)
    {
        return result;
    } 

    if((result = svr->DownloadSong(data)) != SUCCESS)
    {
        return result;
    }

    #endif

    #if 0
    if(svr->InitListenSocket())
    {
        std::cerr << "Can't initiate listen socket" << std::endl;
        delete svr;
        exit(SOCKETERROR);
    }

    if(svr->SetSocketOpt())
    {
        std::cerr << "Can't set the socket operation." << std::endl;
        delete svr;
        exit(SOCKETERROR);
    }

    if(svr->BindSocketAndListen())
    {
        std::cerr << "Socket error, unable to bind and listen." << std::endl;
        delete svr;
        exit(SOCKETERROR);
    }

    //Infinite loop that waits for incoming data.
    if(svr->SelectIncomingData())
    {
        std::cerr << "Socket error, shutting down resources." << std::endl;
        delete svr;
        exit(SOCKETERROR);
    }
    #endif
    delete tube;
    delete svr;
    return 0;
}