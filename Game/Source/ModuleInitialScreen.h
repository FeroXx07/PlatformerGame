#pragma once

#ifndef __MODULEINITIALSCREEN_H__
#define __MODULEINITIALSCREEN_H__

#include "SDL/include/SDL_rect.h"

#include "Module.h"
#include "Textures.h"

struct SDL_Rect;

class ModuleInitialScreen : public Module
{

public:

	ModuleInitialScreen(bool startEnabled);
	~ModuleInitialScreen();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:

	SDL_Texture* logoTex = nullptr;
	SDL_Texture* tex = nullptr;
	SDL_Rect logo;

	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 actualTime = 0;

};

#endif