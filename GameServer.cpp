/*
-----------------------------------------------------------------------------
Filename:    GameServer.cpp
*/

#include "GameServer.h"
#include "ListenServer.h"

//-------------------------------------------------------------------------------------
GameServer::GameServer()
{

}
//-------------------------------------------------------------------------------------
GameServer::~GameServer(void)
{
}

int main(int argc, char *argv[])
{

    GameServer*   gameServer;
    gameServer   = new GameServer();

    ListenServer* listenServer;
    listenServer = new ListenServer();

    //listenServer->setGameServer(gameServer);

    bool gameOn = true;
    while (gameOn == true)
    {
      //  listenServer->processRequests();
    }
}
