#pragma once

#ifndef __MODULEINITIALSCREEN_H__
#define __MODULEINITIALSCREEN_H__

#include "SDL/include/SDL_rect.h"

#include "Module.h"
#include "Textures.h"
#include "Scene.h"

struct SDL_Rect;

class ModuleInitialScreen : public Scene
{

public:

	ModuleInitialScreen();
	~ModuleInitialScreen();

	bool Load(Textures* tex);

	bool Update(Input* input, float dt);

	bool Draw(Render* render);

	bool Unload();

private:

	SDL_Texture* logoTex = nullptr;
	SDL_Texture* tex = nullptr;
	SDL_Rect logo;

	int state;
	float timeCounter;
	float logoAlpha;
};

#endif