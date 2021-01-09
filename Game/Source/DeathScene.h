#ifndef __DEATH_SCENE_H__
#define __DEATH_SCENE_H__

#include "SDL/include/SDL_rect.h"

#include "Module.h"
#include "Textures.h"
#include "Scene.h"
struct SDL_Rect;

class DeathScene : public Scene
{
public:

	DeathScene();
	~DeathScene();

	bool Load(Textures* tex);

	bool Update(Input* input, float dt);

	bool Draw(Render* render);

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool Unload();
private:

	SDL_Texture* logoTex = nullptr;
	SDL_Texture* tex = nullptr;
	SDL_Rect logo;
};



#endif __DEATH_SCENE_H__