#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

/*-------------------------------------------------------------------------------------
--  SOURCE FILE:    Server.h - Chat message redirection server 
--
--  PROGRAM:        svr.out
--
--  FUNCTIONS:      Server(){}
--                  
--                  ~Server(){}
--
--                  int InitListenSocket();
--    
--                  int SetSocketOpt();
--    
--                  int BindSocketAndListen();
--    
--                  int WriteToAllClients(char* data, size_t datasize, int client);
--    
--                  int ReceivePacketFromClient(int client_sd, int index);
--    
--                  void AppendUserNameToMessage(int client, 
--                                               char* msg, 
--                                               size_t* msgSize);
--    
--                  int SelectIncomingData();
--    
--                  int AcceptNewConnection();
--   
--                  void AddUserToConnections(char *name, char *ipAddress, int socket);
--    
--                  int CloseClient(int client_sd, int index);
--    
--                  void SetPort(int _port);
--
--
--  DATE:           Feb 29, 2016
--
--  REVISIONS:      March 13, 2016 (Tyler Trepanier)
--                      Redesign every file to be a C++ class.
--                  March 17, 2016 (Tyler Trepanier)
--                      Added in username functionality to the Client and the
--                      server.
--                  March 18, 2016 (Tyler Trepanier)
--                      Refactoring code removing unnecessary variables.
--
--  DESIGNERS:      Tyler Trepanier
--
--  PROGRAMMERS:    Tyler Trepanier
--
--  NOTES:
--  This program serves as a message redirection server that clients communicate
--  with. All messages passed to this server which will take that message and
--  send it to all the other clients. An ACK message will be sent to the original
--  client who made the message indicating a success. 
--
--  The server will intially create a socket, bind to it and listen for any new
--  connections on that socket. The server makes use of the select function in
--  order to organize which sockets have incoming data, if there are any new
--  connections and any clients who disconnect.
--
--  This header file derives some of the shared includes and definitions inside of
--  packet utilities. 
--
--  The required definitions include:
--      1) SERVER_TCP_PORT      9456
--      2) BUFLEN               255
--      3) MAX_BUFFER           2048
--      4) EOT                  0x04
--      5) ACK                  0x06
-------------------------------------------------------------------------------------*/

#include <strings.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

#define EOT 0x04					// End of Text character to allow for variable length packets.
#define ACK 0x06					// ACK character used to signify successful message sending.

#define SERVER_TCP_PORT 	9654	// Default port
#define BUFLEN			    256     // Standard Buffer length for small messages.
#define MAX_BUFFER          2048    // Maximum buffer size allowed for parsing.
#define PACKET_SIZE			1024	// Standard Packet Size

#define CLOSEDCONNECTION	70		// Client closed their connection
#define CANNOTOPENFILE      60      // Unable to open a file, not a critical error
#define SOCKOPTERROR	    50		// Set socket operation error
#define TOOMANYCLIENTSERROR 40		// Too many current connections
#define BUFFEROVERFLOW      30      // Buffer overflow error
#define SOCKETERROR         20      // Generic socket error
#define BADHOST             10      // Unable to resolve host error
#define SUCCESS             0       // Success

#define LISTENQ	        	5
#define SOCKOPTFLAG     	1

class Server
{
public:
	Server(){}
	~Server(){}

    int InitListenSocket();
    
    int SetSocketOpt();

    int BindSocketAndListen();
    
    int ReceivePacketFromClient(int client_sd, int index);
    
    int ReceiveHeaderFromClient(int client_sd, int index, char* data);

    int ParseHeaderData(char* data);

    int HandleYoutubeRequest(int client_sd, int index);

    int DownloadSong(char* url);

    int SelectIncomingData();
    
    int AcceptNewConnection();
    
    void AddUserToConnections(char *ipAddress, int socket);

    int CloseClient(int client_sd, int index);

private:
    int _maxi;                      //Running total of all clients.
    int _listen_sd;                 //Listening socket
	int _port;                      //Current port to listen
    int _maxfd;                     //Maximum amount of file descriptors      

    struct sockaddr_in _server;     //Server socket address
    struct sockaddr_in _client_addr;//Client socket address
    fd_set _all_set;                //All file descriptors on the listening socket

    //Clients tracked by their username
    std::map<int, std::string>      _clientUsernameMap; 
    
    //Vector of client sockets
    std::vector<int>                _client;      
};

#endif //CHAT_SERVER_H
