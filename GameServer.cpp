/*
-----------------------------------------------------------------------------
Filename:    GameServer.cpp
*/

#include "GameServer.h"


//-------------------------------------------------------------------------------------
GameServer::GameServer()
{

}
//-------------------------------------------------------------------------------------
GameServer::~GameServer(void)
{
}

void GameServer::processClientMessage(char newClientMessage[10])
{
    printf("GameServer: I got this message.... \"%s\"\n", newClientMessage);

}

