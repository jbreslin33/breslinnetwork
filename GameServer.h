/*
-----------------------------------------------------------------------------
Filename:    GameServer.h
*/
#ifndef __GAMESERVER_h_
#define __GAMESERVER_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class GameServer
{

public:
    GameServer();
    virtual ~GameServer(void);

    void processClientMessage(char newClientMessage[10]);

protected:


};

#endif


