
#include "Module.h"
#include "List.h"


class ModuleHud: public Module
{
public:

	ModuleHud(bool isEnabled = false);
	~ModuleHud();
	//By now we will consider all modules to be permanently active
	bool Start();

	//Called at the middle of each application loop
	bool Update();

	//Called at the end of each application loop
	bool PostUpdate();

	//Called at the end of the application
	bool CleanUp();

public:
	uint score = 0;
	uint highScore = 0;
	uint previousScore = 0;

	int whiteFont = -1;
	int redFont = -1;
	int greenFont = -1;

	char scoreText[10] = { "\0" };
	char highScoreText[10] = { "\0" };
	char previousScoreText[10] = { "\0" };
	char livesText[10] = { "\0"};

	bool drawScore = true;
	
	int LevelWins = 0;
};