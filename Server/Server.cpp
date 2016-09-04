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
--                  void AddUserToConnections(char* name, int socket);
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
-------------------------------------------------------------------------------------*/
#include "Server.h"

/*---------------------------------------------------------------------------------
--  FUNCTION:       Initialize Listen Socket
--
--  DATE:           March 13, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::InitListenSocket()
--
--  PARAMETERS:     void
--                      No parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          creating a new TCP Socket.                      
--
--  NOTES:
--  Creates a TCP socket and copies the requested host and port information to 
--  the server instance. Failures in this function are critical to this program.
---------------------------------------------------------------------------------*/
int Server::InitListenSocket()
{
	// Create a stream socket
    if ((_listen_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		return SOCKETERROR;
	}
	return SUCCESS;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Set Socket Operation
--
--  DATE:           March 13, 2016 
--
--  REVISED:        (None)                      
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::SetSocketOpt()
--
--  PARAMETERS:     void
--                      Takes in no parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKOPTERROR (40) when there is an issue with 
--                          setting the socket operation
--
--  NOTES:
--  Enables the computer to reuse the socket for other connections.
---------------------------------------------------------------------------------*/
int Server::SetSocketOpt()
{
	// set SO_REUSEADDR so port can be resused immediately after exit, i.e., after CTRL-c
    int flag = SOCKOPTFLAG;
    if (setsockopt (_listen_sd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1)
	{
		return SOCKOPTERROR;
	}
	return SUCCESS;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Bind Socket And Listen to Socket
--
--  DATE:           March 13, 2016 
--
--  REVISED:        (None)                      
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::SetSocketOpt()
--
--  PARAMETERS:     void
--                      Takes in no parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (10) when there is an issue with 
--                          binding or listening to a socket. Critical failure.
--
--  NOTES:
--  Binds a server to a socket and listens to that socket for any incoming
--  connections.
---------------------------------------------------------------------------------*/
int Server::BindSocketAndListen()
{
	// Bind an address to the socket
    bzero((char *)&_server, sizeof(struct sockaddr_in));
    _server.sin_family = AF_INET;
    _server.sin_port = htons(SERVER_TCP_PORT);
    _server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

    if (bind(_listen_sd, (struct sockaddr *)&_server, sizeof(_server)) == -1)
	{
		return SOCKETERROR;
	}

    if (listen(_listen_sd, LISTENQ) == -1) // queue up to LISTENQ connect requests
	{
		return SOCKETERROR;
	}

	return SUCCESS;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Receive Packet From Client
--
--  DATE:           March 13, 2016 (Tyler Trepanier)
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::ReceivePacketFromClient(int client_sd, 
--                                                      int index
--
--  PARAMETERS:     int client_sd
--                      Client receiving socket
--                  int index
--                      Vector index that contains the client to be closed.
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          receiving from the indicated client socket.
--                      -Returns BUFFEROVERFLOW (30) when there is too much
--                          data coming in.
--
--  NOTES:
--  Receives the contents of a data from pre-connected client. If the contents
--  are unable to be received all at once, it will continously attempt to send
--  until all contents have been received or there is an error.
--
--  There are three conditions that will stop a client from receiving data:
--      1) The server is indicating that this client has successfully broadcasted
--         their message.
--      2) The server has sent a message from another client and is relaying the
--         message to this client.
--      3) There is an overflow of data and the client will return an error
--         indicating that there is too much garbage data being received.
---------------------------------------------------------------------------------*/
int Server::ReceivePacketFromClient(int client_sd, int index)
{
	char 	buf[MAX_BUFFER];	    //Container for incoming message.
	char 	*bp = buf;		        //Pointer to the receiving buffer.
	size_t	bytes_to_read = 0;	    //Ensures that the buffer will not overflow
    size_t  total_bytes_read = 0;   //Running total of the bytes received.
    size_t  n = 0;                  //Keeps track of the incoming bytes.

	bytes_to_read = MAX_BUFFER;

	//Keep reading until you reach an EOT at the end of the packet
	while ((n = recv(client_sd, bp, bytes_to_read, 0)) > 0)
	{
        total_bytes_read += n;

		if(buf[total_bytes_read-1] == EOT)
        {
            buf[total_bytes_read-1] = '\0';
            total_bytes_read--;
            break;
        }

        if(bytes_to_read == 0) // BUFFEROVERFLOW
        {
            return BUFFEROVERFLOW;
        }

        bp += n;
		bytes_to_read -= n;
	}

	if(total_bytes_read == 0 || n == 0) //The client has disconnected.
	{
		CloseClient(client_sd, index);
	}

	return SUCCESS;
}

int Server::ReceiveHeaderFromClient(int client_sd, int index, char* data)
{
    char    *bp = data;             //Pointer to the receiving buffer.
    size_t  bytes_to_read = 0;      //Ensures that the buffer will not overflow
    size_t  total_bytes_read = 0;   //Running total of the bytes received.
    size_t  n = 0;                  //Keeps track of the incoming bytes.

    bytes_to_read = PACKET_SIZE;

    /*
    Plan: [option][URL]
        option: 0 = this packet only ; 1 = more than one packet 
        URL:    url to query
    */

    //Keep reading until you reach an EOT at the end of the packet
    while ((n = recv(client_sd, bp, bytes_to_read, 0)) > 0)
    {
        total_bytes_read += n;

        if(bytes_to_read == 0) // BUFFEROVERFLOW
        {
            return BUFFEROVERFLOW;
        }

        bp += n;
        bytes_to_read -= n;
    }

    if(total_bytes_read == 0 || n == 0) //The client has disconnected.
    {
        CloseClient(client_sd, index);
        return CLOSEDCONNECTION;
    }

    return SUCCESS;
}

int Server::ParseHeaderData(char* data)
{
    int option = 0;
    char url[256];
    
    if(sscanf(data, "[%d][%s]", &option, url) != 2)
    {
        return -1;
    }

    if(option != 0)
    {
        return -1;
    }
    memcpy(data, url, strlen(url));

    return SUCCESS;
}

int Server::HandleYoutubeRequest(int client_sd, int index)
{
    int result = 0;
    char* data = (char*) malloc(PACKET_SIZE);

    if((result = ReceiveHeaderFromClient(_client[index], index, data)) != SUCCESS)
    {
        return result;
    }

    if((result = ParseHeaderData(data)) != SUCCESS)
    {
        return result;
    } 

    if((result = DownloadSong(data)) != SUCCESS)
    {
        return result;
    }

    return SUCCESS;
}

int Server::DownloadSong(char* url)
{
    //" \"fulltitle\": "
    return SUCCESS;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Select Incoming Data
--
--  DATE:           March 13, 2016 (Tyler Trepanier)
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::SelectIncomingData()
--
--  PARAMETERS:     void
--                      No parameters used
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          setting the socket operation.
--
--  NOTES:
--  This function initializes all sockets with bad values so it can be filled
--  with new client values and the number of connected clients will be kept in
--  check with _maxi.
--
--  Blocks all sockets and waits for any new incoming information from those
--  sockets. All new connections will be handled by the AcceptNewConnection
--  while already connected clients will have their packet received by the
--  ReceivePacketFromClient function.
---------------------------------------------------------------------------------*/
int Server::SelectIncomingData()
{
    fd_set rset;    // Temporary instance of _all_set used for select()
    int nready;     // Number of sockets that have received data.
    int error;      // Error code to be diagnosed.

    //Initialize the Vector with bad sockets
    std::vector<int> temp(FD_SETSIZE, -1);
	_client = temp;

	_maxfd	= _listen_sd;	// initialize
    _maxi	= -1;			// index into client[] array

    FD_ZERO(&_all_set);
    FD_SET(_listen_sd, &_all_set);

    std::cerr << "Going into select mode." << std::endl;

	while (true)
    {
        rset = _all_set;               // structure assignment
        nready = select(_maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(_listen_sd, &rset)) // new client connection
        {
			if((error = AcceptNewConnection()) > 0)
            {
                if(error == TOOMANYCLIENTSERROR) //Acceptable error, ignore request.
                {
                    std::cerr << "Too many clients, connection refused." << std::endl;
                }
                else if(error == SOCKETERROR)
                {
                    std::cerr << "Client was unable to connected." << std::endl;
                }
                else if(error == BUFFEROVERFLOW)
                {
                    std::cerr << "Corruption occured with client data. Connection refused." << std::endl;
                }
            }

			// Check if there are no more interactions
			if (--nready <= 0)
				continue;
        }

        for (int i = 0; i <= _maxi; i++)	// check all clients for data
        {
            if (_client[i] < 0)
                continue;

            if (FD_ISSET(_client[i], &rset))
            {
				
                //ReceivePacketFromClient(_client[i], i);
                HandleYoutubeRequest(_client[i], i);

				// Check if there are no more interactions
				if (--nready <= 0)
                    break;
            }
        } //End of for loop

    } //End of while(true) loop

	return SUCCESS;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Accept new connection
--
--  DATE:           March 13, 2016 
--
--  REVISED:        (None)                      
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::AcceptNewConnection()
--
--  PARAMETERS:     void
--                      Takes in no parameters
--
--  RETURNS:        int error
--                      -Returns 0 when there is no error with Program execution
--                      -Returns SOCKETERROR (20) when there is an issue with 
--                          accepting a new client.
--
--  NOTES:
--  Attempts to connect to a client. All sucessful connections are made public 
--  via the console printing out the client's IP Address. Before completely 
--  accepting the new connection, the server will receive the client's requested
--  username. Any issues accepting/receiving/sending will result in a socket
--  error and the client's connection will be refused.
---------------------------------------------------------------------------------*/
int Server::AcceptNewConnection()
{
	int new_sd = 0;                 // New client socket.
    int i;                          // Index for iterating through _client vector
    size_t bytes;                   // Bytes received from receiving
    char buf[BUFLEN] = {'\0'};      // Receive buffer
    char eot[1];                    // EOT character to indicate a succesful read

	socklen_t client_len = sizeof(_client_addr);

	if ((new_sd = accept(_listen_sd, (struct sockaddr *) &_client_addr, &client_len)) == -1)
	{
		return SOCKETERROR;
	}

    /*
    Before we accept the client, check to see if we have reach the maximum
    amount of connections.
    */
    #if 0
    for (i = 0; i < FD_SETSIZE; i++)
	{
		if (_client[i] < 0)
		{
			_client[i] = new_sd;	// save descriptor
			break;
		}
	}

	if (i == FD_SETSIZE)
	{
		close(new_sd);
		return TOOMANYCLIENTSERROR;
	}
    #endif

    #if 1
    for (i = 0; i < 1; i++)
    {
        if (_client[i] < 0)
        {
            _client[i] = new_sd;    // save descriptor
            break;
        }
    }

    if (i == 1)
    {
        close(new_sd);
        return TOOMANYCLIENTSERROR;
    }
    #endif

    AddUserToConnections(inet_ntoa(_client_addr.sin_addr), new_sd);

	FD_SET (new_sd, &_all_set); 	// add new descriptor to set

	if (new_sd > _maxfd)
		_maxfd = new_sd;			// For select

	if (i > _maxi)
		_maxi = i;				// new max index for client vector

	return SUCCESS;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Close Client Connection
--
--  DATE:           March 13, 2016
--
--  REVISED:        March 17, 2016 (Tyler Trepanier)
--                      Added in username functionality to the Client and the
--                      Server.
--                  March 23, 2016 (Tyler Trepanier)
--                      Broadcast to all clients a disconnection.
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      int Server::CloseClient(int client_sd, int index)
--
--  PARAMETERS:     int client_sd
--                      Connected client socket to be released
--                  int index
--                      Index in the _client vector
--
--  RETURNS:        int
--                      -Returns 0 on SUCCESS
--
--  NOTES:
--  Closes the TCP socket, broadcasts to all clients that a client has
--  disconnected and frees all disconnected client's used resources.
---------------------------------------------------------------------------------*/
int Server::CloseClient(int client_sd, int index)
{
    close(client_sd);
    FD_CLR(client_sd, &_all_set);
    _client[index] = -1;
    --_maxi;
    
    _clientUsernameMap.erase(client_sd);

    return SUCCESS;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:       Add User To Connections
--
--  DATE:           March 17, 2016
--
--  REVISED:        (None)
--
--  DESIGNER:       Tyler Trepanier
--
--  PROGRAMMER:     Tyler Trepanier
--
--  INTERFACE:      void Server::AddUserToConnections(char *ipAddress, int socket)
--
--  PARAMETERS:     char *ipAddress 
--                      Client's ip address.                                   
--                  int socket
--                      Socket used as the key in the client username map.
--
--  RETURNS:        void
--                      No return value.
--
--  NOTES:
--  Adds in the client username to the client username map and uses their socket
--  as the key.
---------------------------------------------------------------------------------*/
void Server::AddUserToConnections(char *ipAddress, int socket)
{
    std::string temp(ipAddress);
    _clientUsernameMap.insert(std::make_pair(socket, temp));
}

// Initializes the socket to prepare for sending/receiving.
int Server::OpenSocket()
{
    int ret = SUCCESS;

    if((ret = InitListenSocket() != SUCCESS)) 
    {
        _error = std::string("Can't initiate listen socket.");
        return ret;
    }

    if((ret = SetSocketOpt() != SUCCESS))
    {
        _error = std::string("Can't set the socket operation.");
        return ret;
    }

    if((ret = BindSocketAndListen() != SUCCESS))
    {
        _error = std::string("Socket error, unable to bind and listen.");
        return ret;
    }

    return ret;
}

// Clean up server resources here.
int Server::CloseSocket()
{
    int ret = SUCCESS;

    /*
    TODO:
    1) Close all of the current connections to clients.
    2) Remove all of the clients from the list.
    3) Free any created resources
    4) Any errors in the above steps will set an error code but will always
        close the listen socket.
    */

    close(_listen_sd);

    return ret;
}