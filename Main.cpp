#include "GameServer.h"
#include "ListenServer.h"

#include <iostream>
#include <string>

int main()
{
    std::cout << "Launching GameServer..." << std::endl;

    GameServer*   gameServer   = new GameServer  ();
    ListenServer* listenServer = new ListenServer();

    listenServer->setGameServer(gameServer);


    bool gameOn = true;
    while (gameOn == true)
    {
        listenServer->processRequests();
    }

}
