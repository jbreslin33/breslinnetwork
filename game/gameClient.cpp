#include "gameClient.h"

GameClient* game;
bool keys[256];

void GameClient::createPlayer(int index)
{
	Ogre::Entity* NinjaEntity = mSceneMgr->createEntity("ninja.mesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(NinjaEntity);
    clientData *client = game->GetClientPointer(index);
	client->myNode = node;
}
void GameClient::createServerPlayer(int index)
{
    Ogre::Entity* NinjaEntity = mSceneMgr->createEntity("ninja.mesh");
    Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(NinjaEntity);
    clientData *client = game->GetClientPointer(index);
    client->myServerNode = node;
    LogString("createServerPlayer index:%d",index);
}
//-------------------------------------------------------------------------------------
void GameClient::createScene(void)
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.75, 0.75, 0.75));

    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(250, 150, 250));
    pointLight->setDiffuseColour(Ogre::ColourValue::White);
    pointLight->setSpecularColour(Ogre::ColourValue::White);
}
//-------------------------------------------------------------------------------------
bool GameClient::processUnbufferedInput(const Ogre::FrameEvent& evt)
{
    if (mKeyboard->isKeyDown(OIS::KC_I)) // Forward
    {
		keys[VK_UP] = TRUE;
    }
	else
	{
        keys[VK_UP] = FALSE;
	}
    if (mKeyboard->isKeyDown(OIS::KC_K)) // Backward
    {
		keys[VK_DOWN] = TRUE;
    }
	else
	{
        keys[VK_DOWN] = FALSE;
	}
    if (mKeyboard->isKeyDown(OIS::KC_J)) // Left - yaw or strafe
    {
		keys[VK_LEFT] = TRUE;
    }
	else
	{
        keys[VK_LEFT] = FALSE;
	}
    if (mKeyboard->isKeyDown(OIS::KC_L)) // Right - yaw or strafe
    {
		keys[VK_RIGHT] = TRUE;
    }
	else
	{
        keys[VK_RIGHT] = FALSE;
	}

	if (mKeyboard->isKeyDown(OIS::KC_V)) // toggle serverPlayerMovement
	{
		mServerPlayerToggle = !mServerPlayerToggle;
	}

    return true;
}
//-------------------------------------------------------------------------------------
bool GameClient::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);

    if(!processUnbufferedInput(evt)) return false;

	if(game != NULL)
	{
		game->CheckKeys();
		game->RunNetwork(evt.timeSinceLastFrame * 1000);
		rendertime = evt.timeSinceLastFrame;
	}
    return ret;
}

GameClient::GameClient(const char* serverIP)
{
	mServerIP = serverIP;
 	networkClient	= new dreamClient;
 	clientList		= NULL;
 	localClient		= NULL;
 	memset(&inputClient, 0, sizeof(clientData));
 	frametime		= 0.0f;
 	rendertime		= 0.0f;
 	init			= false;
	mServerPlayerToggle = false;
 }

 GameClient::~GameClient()
 {
 	delete networkClient;
 }

 void GameClient::Shutdown(void)
 {
 	Disconnect();
 }

 clientData *GameClient::GetClientPointer(int index)
 {
 	for(clientData *clList = clientList; clList != NULL; clList = clList->next)
 	{
 		if(clList->index == index)
 			return clList;
 	}
 	return NULL;
 }

void GameClient::CheckKeys(void)
{
	inputClient.command.key = 0;
 	if(keys[VK_ESCAPE])
 	{
 		Shutdown();
 		keys[VK_ESCAPE] = false;
 	}
 	if(keys[VK_DOWN])
 	{
 		inputClient.command.key |= KEY_DOWN;
 	}
 	if(keys[VK_UP])
 	{
 		inputClient.command.key |= KEY_UP;
 	}
 	if(keys[VK_LEFT])
 	{
 		inputClient.command.key |= KEY_LEFT;
 	}
 	if(keys[VK_RIGHT])
 	{
 		inputClient.command.key |= KEY_RIGHT;
 	}
 	inputClient.command.msec = (int) (frametime * 1000);
 }

 void GameClient::CalculateVelocity(command_t *command, float frametime)
 {
 	float multiplier = 100.0f;

 	command->vel.x = 0.0f;
 	command->vel.y = 0.0f;

 	if(command->key & KEY_UP)
 	{
 		command->vel.y += multiplier * frametime;
 	}
 	if(command->key & KEY_DOWN)
 	{
 		command->vel.y += -multiplier * frametime;
 	}
 	if(command->key & KEY_LEFT)
 	{
 		command->vel.x += -multiplier * frametime;
 	}
 	if(command->key & KEY_RIGHT)
 	{
 		command->vel.x += multiplier * frametime;
 	}
 }

void GameClient::MoveRemotePlayers(void)
{
 	if(!localClient)
		return;

	clientData *client = clientList;
 
 	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;

 	for( ; client != NULL; client = client->next)
 	{
		CalculateVelocity(&client->command, rendertime);

 		client->command.origin.x += client->command.vel.x;
 		client->command.origin.y += client->command.vel.y;

        //transVector.x = client->command.origin.x;
        //transVector.y = client->command.origin.y;

		transVector.x = client->command.vel.x;
		transVector.y = client->command.vel.y;
			
		//client->myNode->setPosition(transVector * rendertime);
	    client->myNode->translate  (transVector, Ogre::Node::TS_WORLD);
	}
 }

void GameClient::MoveServerPlayer(void)
{
	if(!localClient)
		return;
 
    Ogre::Vector3 transVector = Ogre::Vector3::ZERO;

    transVector.x = localClient->serverFrame.origin.x;
    transVector.y = localClient->serverFrame.origin.y;
	
	if (mServerPlayerToggle)
		localClient->myServerNode->setPosition(transVector);
}

void GameClient::StartConnection()
{
	int ret = networkClient->Initialise("", mServerIP, 30004);

	if(ret == DREAMSOCK_CLIENT_ERROR)
	{
		char text[64];
		sprintf(text, "Could not open client socket");
	}
	Connect();
}

void GameClient::ReadPackets(void)
{
	char data[1400];
	struct sockaddr address;

	clientData *clList;

	int type;
	int ind;
	int local;
	int ret;

	char name[50];

	dreamMessage mes;
	mes.Init(data, sizeof(data));

	while(ret = networkClient->GetPacket(mes.data, &address))
	{
		LogString("got message!");
		mes.SetSize(ret);
		mes.BeginReading();

		type = mes.ReadByte();

		switch(type)
		{
		case DREAMSOCK_MES_ADDCLIENT:
			local	= mes.ReadByte();
			ind		= mes.ReadByte();
			strcpy(name, mes.ReadString());

			AddClient(local, ind, name);
			break;

		case DREAMSOCK_MES_REMOVECLIENT:
			ind = mes.ReadByte();

			LogString("Got removeclient %d message", ind);

			RemoveClient(ind);

			break;

		case USER_MES_FRAME:
			// Skip sequences
			mes.ReadShort();
			mes.ReadShort();

			for(clList = clientList; clList != NULL; clList = clList->next)
			{
				LogString("Reading DELTAFRAME for client %d", clList->index);
				ReadDeltaMoveCommand(&mes, clList);
			}

			break;

		case USER_MES_NONDELTAFRAME:
			// Skip sequences
			mes.ReadShort();
			mes.ReadShort();

			clList = clientList;

			for(clList = clientList; clList != NULL; clList = clList->next)
			{
				LogString("Reading NONDELTAFRAME for client %d", clList->index);
				ReadMoveCommand(&mes, clList);
			}

			break;

		case USER_MES_SERVEREXIT:
			//MessageBox(NULL, "Server disconnected", "Info", MB_OK);
			Disconnect();
			break;

		}
	}
}

void GameClient::AddClient(int local, int ind, char *name)
{
	// First get a pointer to the beginning of client list
	clientData *list = clientList;
	clientData *prev;

	LogString("App: Client: Adding client with index %d", ind);

	// No clients yet, adding the first one
	if(clientList == NULL)
	{
		LogString("App: Client: Adding first client");

		clientList = (clientData *) calloc(1, sizeof(clientData));

		if(local)
		{
			LogString("App: Client: This one is local");
			localClient = clientList;
		}

		clientList->index = ind;
		strcpy(clientList->nickname, name);

		createPlayer(ind);
		clientList->next = NULL;
        if (local)
			createServerPlayer(ind);
	}
	else
	{
		LogString("App: Client: Adding another client");

		prev = list;
		list = clientList->next;

		while(list != NULL)
		{
			prev = list;
			list = list->next;
		}

		list = (clientData *) calloc(1, sizeof(clientData));

		if(local)
		{
			LogString("App: Client: This one is local");
			localClient = list;
		}

		list->index = ind;
		strcpy(list->nickname, name);

		clientList->next = NULL;

		list->next = NULL;
		prev->next = list;

		createPlayer(ind);
        if (local)
			createServerPlayer(ind);
	}

	// If we just joined the game, request a non-delta compressed frame
	if(local)
		SendRequestNonDeltaFrame();
}

void GameClient::RemoveClient(int ind)
{
	clientData *list = clientList;
	clientData *prev = NULL;
	clientData *next = NULL;

	// Look for correct client and update list
	for( ; list != NULL; list = list->next)
	{
		if(list->index == ind)
		{
			if(prev != NULL)
			{
				prev->next = list->next;
			}

			break;
		}

		prev = list;
	}

	// First entry
	if(list == clientList)
	{
		if(list)
		{
			next = list->next;
			free(list);
		}

		list = NULL;
		clientList = next;
	}

	// Other
	else
	{
		if(list)
		{
			next = list->next;
			free(list);
		}
		list = next;
	}
}

void GameClient::RemoveClients(void)
{
	clientData *list = clientList;
	clientData *next;

	while(list != NULL)
	{
		if(list)
		{
			next = list->next;
			free(list);
		}
		list = next;
	}
	clientList = NULL;
}

void GameClient::SendCommand(void)
{
	if(networkClient->GetConnectionState() != DREAMSOCK_CONNECTED)
		return;

	dreamMessage message;
	char data[1400];

	int i = networkClient->GetOutgoingSequence() & (COMMAND_HISTORY_SIZE-1);

	message.Init(data, sizeof(data));
	message.WriteByte(USER_MES_FRAME);						// type
	message.AddSequences(networkClient);					// sequences

	// Build delta-compressed move command
	BuildDeltaMoveCommand(&message, &inputClient);

	// Send the packet
	networkClient->SendPacket(&message);

	// Store the command to the input client's history
	memcpy(&inputClient.frame[i], &inputClient.command, sizeof(command_t));

	clientData *clList = clientList;

	// Store the commands to the clients' history
	for( ; clList != NULL; clList = clList->next)
	{
		memcpy(&clList->frame[i], &clList->command, sizeof(command_t));
	}
}

void GameClient::SendRequestNonDeltaFrame(void)
{
	char data[1400];
	dreamMessage message;
	message.Init(data, sizeof(data));

	message.WriteByte(USER_MES_NONDELTAFRAME);
	message.AddSequences(networkClient);

	networkClient->SendPacket(&message);
}

void GameClient::Connect(void)
{
	if(init)
	{
		LogString("ArmyWar already initialised");
		return;
	}

	LogString("GameClient::Connect");

	init = true;

	networkClient->SendConnect("myname");
}

void GameClient::Disconnect(void)
{
	if(!init)
		return;

	LogString("GameClient::Disconnect");

	init = false;
	localClient = NULL;
	memset(&inputClient, 0, sizeof(clientData));

	networkClient->SendDisconnect();
}

void GameClient::ReadMoveCommand(dreamMessage *mes, clientData *client)
{
	// Key
	client->serverFrame.key				= mes->ReadByte();

	// Origin
	client->serverFrame.origin.x		= mes->ReadFloat();
	client->serverFrame.origin.y		= mes->ReadFloat();
	client->serverFrame.vel.x			= mes->ReadFloat();
	client->serverFrame.vel.y			= mes->ReadFloat();

	//Read realtime and ticknumber
	client->serverFrame.realtime = mes->ReadShort();
	client->serverFrame.framenum = mes->ReadLong();

	// Read time to run command
	client->serverFrame.msec = mes->ReadByte();

	memcpy(&client->command, &client->serverFrame, sizeof(command_t));

	// Fill the history array with the position we got
	for(int f = 0; f < COMMAND_HISTORY_SIZE; f++)
	{
		client->frame[f].predictedOrigin.x = client->command.origin.x;
		client->frame[f].predictedOrigin.y = client->command.origin.y;
	}
}

void GameClient::ReadDeltaMoveCommand(dreamMessage *mes, clientData *client)
{
	int processedFrame;
	int flags = 0;

	// Flags
	flags = mes->ReadByte();

	// Key
	if(flags & CMD_KEY)
	{
		client->serverFrame.key = mes->ReadByte();

		client->command.key = client->serverFrame.key;
		LogString("Client %d: Read key %d", client->index, client->command.key);
	}

	if(flags & CMD_ORIGIN)
	{
		processedFrame = mes->ReadByte();
	}

	// Origin
	if(flags & CMD_ORIGIN)
	{
		LogString("Recieved CMD_ORIGIN From client %d:", client->index);
		client->serverFrame.origin.x = mes->ReadFloat();
		client->serverFrame.origin.y = mes->ReadFloat();

		client->serverFrame.vel.x = mes->ReadFloat();
		client->serverFrame.vel.y = mes->ReadFloat();

		client->command.origin.x = client->serverFrame.origin.x;
		client->command.origin.y = client->serverFrame.origin.y;

		client->command.vel.x = client->serverFrame.vel.x;
		client->command.vel.y = client->serverFrame.vel.y;

		//Read realtime and ticknumber
		client->command.realtime = client->serverFrame.realtime;
		client->command.framenum = client->serverFrame.framenum;
	}

	//Read realtime and ticknumber
	client->serverFrame.realtime = mes->ReadShort();
	client->serverFrame.framenum = mes->ReadLong();

	// Read time to run command
	LogString("Read Delta Move for Client %d: msec: %d", client->index, client->command.msec);
	client->command.msec = mes->ReadByte();
}

//-----------------------------------------------------------------------------
// Name: empty()
// Desc:
//-----------------------------------------------------------------------------
void GameClient::BuildDeltaMoveCommand(dreamMessage *mes, clientData *theClient)
{
	int flags = 0;
	int last = (networkClient->GetOutgoingSequence() - 1) & (COMMAND_HISTORY_SIZE-1);

	// Check what needs to be updated
	if(theClient->frame[last].key != theClient->command.key)
		flags |= CMD_KEY;

	// Add to the message
	// Flags
	mes->WriteByte(flags);

	// Key
	if(flags & CMD_KEY)
	{
		mes->WriteByte(theClient->command.key);
	}

	mes->WriteByte(theClient->command.msec);
}

//-----------------------------------------------------------------------------
// Name: empty()
// Desc:
//-----------------------------------------------------------------------------
void GameClient::RunNetwork(int msec)
{
	static int time = 0;
	time += msec;

	MoveServerPlayer();
	MoveRemotePlayers();

	// Framerate is too high
	if(time < (1000 / 60)) {
		return;
	}

	frametime = time / 1000.0f;
	time = 0;

	// Read packets from server, and send new commands
	ReadPackets();
	SendCommand();
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {

        //ClientSideBaseGame* mClientSideBaseGame;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        game = new GameClient(strCmdLine);
#else
        game = new GameClient(argv[1]);
#endif

		//game = new GameClient;
	    game->StartConnection();

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		StartLogConsole();
#endif

        try {
            game->go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif