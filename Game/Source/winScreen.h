#ifndef __WIN_SCREEN_H__
#define __WIN_SCREEN_H__

#include "SDL/include/SDL_rect.h"

#include "Module.h"
#include "Textures.h"
#include "Scene.h"

struct SDL_Rect;

class WinScreen : public Scene
{

public:

	WinScreen();
	~WinScreen();

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



#endif __WIN_SCREEN_H__