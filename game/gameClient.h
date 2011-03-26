#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "BaseApplication.h"
#include "../tdreamsock/dreamSock.h"

#define VK_ESCAPE 0x1B
#define VK_UP 0x26
#define VK_DOWN 0x28
#define VK_LEFT 0x25
#define VK_RIGHT 0x27

extern bool keys[256];

#define COMMAND_HISTORY_SIZE	64

#define KEY_UP					1
#define KEY_DOWN				2
#define KEY_LEFT				4
#define KEY_RIGHT				8

#define CMD_KEY					1
#define CMD_ORIGIN				4

#define USER_MES_FRAME			1
#define USER_MES_NONDELTAFRAME	2
#define USER_MES_SERVEREXIT		3

#define USER_MES_KEEPALIVE		12

typedef struct clientLoginData
{
	int					index;
	char				nickname[30];
	clientLoginData	*next;
} clientLoginData;

typedef struct
{
	float x;
	float y;
} VECTOR2D;

typedef struct
{
	int			key;

	VECTOR2D	vel;
	VECTOR2D	origin;
	VECTOR2D	predictedOrigin;

	int         realtime;
	long        framenum;

	int			msec;
} command_t;

typedef struct clientData
{
	command_t	frame[64];	// frame history
	command_t	serverFrame[10];					// the latest frame from server
	command_t	command;                           // current frame's commands
	command_t   serverPlayer;

	int			index;
	
	VECTOR2D	startPos;
	bool		team;
	char		nickname[30];
	char		password[30];

	Ogre::SceneNode *myNode;
	Ogre::SceneNode *myServerNode;

	clientData	*next;
} clientData;

class GameClient : public BaseApplication
{
public:
	GameClient(const char* serverIP);
	~GameClient();

	//Players
	void	CalculateVelocity(command_t *command, float frametime);
    void	MoveRemotePlayers(void);
    void	MoveServerPlayer(void);
	//Game
	void	AddClient(int local, int index, char *name);
	void	RemoveClient(int index);
	void	RemoveClients(void);
	void	Shutdown(void);
	void	CheckKeys(void);
	void	Frame(void);
	void	RunNetwork(int msec);

	// Network
	void	ReadPackets(void);
	void	SendCommand(void);
	void	SendRequestNonDeltaFrame(void);
	void	ReadMoveCommand(dreamMessage *mes, clientData *client);
	void	ReadDeltaMoveCommand(dreamMessage *mes, clientData *client);
	void	BuildDeltaMoveCommand(dreamMessage *mes, clientData *theClient);
	void	StartConnection();
	void	Connect(void);
	void	Disconnect(void);
	void	SendStartGame(void);

	//Ogre
	bool processUnbufferedInput(const Ogre::FrameEvent& evt);
    void createPlayer(int index);
	void createServerPlayer(int index);
    virtual void createScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// Network variables
	dreamClient *networkClient;
	const char* mServerIP;

	//client Variables
	clientData *clientList;			// Client list
	clientData *localClient;		// Pointer to the local client in the client list
	clientData inputClient;			// Handles all keyboard input
	clientData *GetClientList(void) { return clientList; }
	clientData *GetClientPointer(int index);

	//time
	float frametime;
	float rendertime;
	int	  tickIndex;

	bool init;

	//serverPlayer
	bool mServerPlayerToggle;
};

#endif