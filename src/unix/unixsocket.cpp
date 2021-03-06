/*--------------------------------------

    Copyright Vlad Ionut Ursachi 2016
    Telnet based C++ Chat Server

----------------------------------------*/

#include "platform.h"
#include "unixsocket.h"

fd_set Socket::readfds;

//Initialized socket system
int Socket::globalInit()
{
    //no global init needed on unix
    return CS_OK;
}

//Exist socket system
int Socket::globalShutdown()
{
    //no global shutdown needed on unix
    return CS_OK;
}

//create listening socket
SOCKET Socket::createServerSocket(int port_number)
{
    //try to create the socket
    SOCKET hServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if ( hServerSocket == (SOCKET)(-1) )
    {
        return (SOCKET)(-1);
    }

    // Create the structure for binding the socket to the listening port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;     
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serverAddr.sin_port = htons((uint32_t)port_number);

    // Bind the Server socket to the address & port
    if ( bind( hServerSocket, ( struct sockaddr * ) &serverAddr, sizeof( serverAddr ) ) == -1 )
    {
        // Free the socket and cleanup the environment initialized by WSAStartup()
        close(hServerSocket);
        return (SOCKET)(-1);
    }

    // Put the Server socket in listen state so that it can wait for client connections
    if ( listen( hServerSocket, SOMAXCONN ) == -1 )
    {
        // Free the socket and cleanup the environment initialized by WSAStartup()
        close(hServerSocket);
        return (SOCKET)(-1);
    }

    return hServerSocket;
}

//accept a new connection on a socket
//blocking if no incoming connection
SOCKET Socket::acceptConnection(SOCKET listening_socket)
{
    SOCKET new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    if ((new_socket = accept(listening_socket, (struct sockaddr *)&address, &addrlen)) != -1)
        return new_socket;
    else
        return (SOCKET)(-1);
}

//close a socket
void Socket::closeTheSocket(SOCKET sock)
{
    close(sock);
}

//do a select waiting for an event on a set of sockets
int Socket::doSelect(std::set<SOCKET>& localsockets)
{
    std::set<SOCKET>::iterator i;
    SOCKET sock;
    int max_sd = 0;
    int activity;

    //clear the socket set
    FD_ZERO(&readfds);

    if (localsockets.begin() != localsockets.end())
    {
        //go through each socket and add them
        for (i = localsockets.begin(); i != localsockets.end(); i++)
        {
            sock = *i;
            FD_SET(sock, &readfds);
        }
        //go to last socket
        i--;
        //its value would be maximum due to set being ordered
        max_sd = *i;

        //blocking function
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
    }
    else
        return -1;

    return activity;
}

//check if a socket was set by select
int Socket::isSocketSet(SOCKET& sock)
{
    return FD_ISSET(sock, &readfds);
}

//block and wait for receiving data on a socket
int Socket::receiveOnSocket(SOCKET sock, char* buffer, int buff_size)
{
    return read(sock, buffer, buff_size );
}

//send data through a socket
int Socket::sendOnSocket(SOCKET sock, const char* buffer, int buff_size)
{
    return write( sock, buffer, buff_size );
}
