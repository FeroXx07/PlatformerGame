#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "Map.h"
#include "Scene.h"

struct SDL_Texture;
class Player;

class LevelScene : public Scene
{
public:
	bool Load(Textures* tex);

	bool Update(Input* input, float dt);

	bool Draw(Render* render);

	bool Unload();

	LevelScene();

	LevelScene(bool load = false);

	// Destructor
	virtual ~LevelScene();

	int resetCounter = 0;

private:
	SDL_Texture* img;
	SDL_Texture* bg;
public:
	Player* player = nullptr;
	Map* map = nullptr;

	bool toLoad = false;

	Timer timerSec;
	Font* font;
	char counterText[10] = { "\0" };
};

#endif // __LEVEL_SCENE_H__