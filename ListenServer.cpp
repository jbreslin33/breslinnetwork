/*
-----------------------------------------------------------------------------
Filename:    ListenServer.cpp
*/

#include "ListenServer.h"

//-------------------------------------------------------------------------------------
ListenServer::ListenServer()
{
    mMaxBufferLength = MAXBUFLEN;
    mPort = "4950";
    initializeVariables();
    initializeListener();
}
//-------------------------------------------------------------------------------------
ListenServer::~ListenServer(void)
{
}

int main(int argc, char *argv[])
{
/*
    ListenServer* listenServer = new ListenServer("4950");

    bool listenOn = true;
    while (listenOn == true)
    {
        listenServer->processRequests();
    }
    */
}

void *ListenServer::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void ListenServer::initializeVariables()
{
    mGameServer = NULL;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
}


bool ListenServer::initializeListener()
{
    if ((rv = getaddrinfo(NULL, mPort, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    printf("listener: waiting to recvfrom...\n");
}


void ListenServer::processRequests()
{
    //Did something go wrong?
    addr_len = sizeof their_addr;

    char newClientMessage[mMaxBufferLength];

    for (int i = 0; i < mMaxBufferLength; i++)
    newClientMessage[i] = NULL;

    if ((numbytes = recvfrom(sockfd, newClientMessage, mMaxBufferLength-1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }
    printf("listener: packet contains \"%s\"\n", newClientMessage);

    if (mGameServer != NULL)
    {

        printf("We have a Game Server\n");
    }
    else
    {
        printf("No Game Server, do nothing\n");
    }
}
