
#include "Module.h"
#include "List.h"

struct SDL_Rect;
struct SDL_Texture;

class ModuleHud: public Module
{
public:

	ModuleHud(bool isEnabled = false);
	~ModuleHud();
	//By now we will consider all modules to be permanently active
	bool Start();

	//Called at the middle of each application loop
	bool Update(float dt);

	//Called at the end of each application loop
	bool PostUpdate();

	//Called at the end of the application
	bool CleanUp();

public:

	SDL_Texture* itemsTexture = nullptr;
	SDL_Rect spriteStar;
	SDL_Rect spriteHealth;

	int lifesFont = -1;
	int starsFont = -1;

	char livesText[10] = { "\0"};
	char starText[10] = { "\0" };
	char healthText[10] = { "\0" };

	int drawScore = 0;
};