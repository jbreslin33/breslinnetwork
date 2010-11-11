/*
-----------------------------------------------------------------------------
Filename:    GameServer.h
*/
#ifndef __GAMESERVER_h_
#define __GAMESERVER_h_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXBUFLEN 10;


class GameServer
{



public:
    GameServer(const char* port);
    virtual ~GameServer(void);

    void *get_in_addr(struct sockaddr *sa);
    void initializeVariables();
    bool initializeListener();
    void processRequests();


protected:

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
    const char* mPort;
    int mMaxBufferLength;
};

#endif


