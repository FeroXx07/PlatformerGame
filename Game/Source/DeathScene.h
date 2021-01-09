#pragma once

#ifndef __DEATH_SCENE_H__
#define __DEATH_SCENE_H__

#include "SDL/include/SDL_rect.h"
#ifdef _DEBUG
#pragma comment( lib, "Source/External/SDL/include/SDL_rect.h")
#else
#pragma comment( lib, "Source/External/SDL/include/SDL_rect.h")
#endif

#include "Module.h"
#include "Textures.h"

struct SDL_Rect;

class DeathScene : public Module
{

public:

	DeathScene(bool startEnabled);
	~DeathScene();

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



#endif __DEATH_SCENE_H__