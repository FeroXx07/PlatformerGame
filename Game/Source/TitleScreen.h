#ifndef __SCENE2_H__
#define __SCENE2_H__

#include "Module.h"
#include "Scene.h"

#include "GuiButton.h"
#include "Animation.h"

struct SDL_Rect;

class TitleScreen : public Scene
{
public:
	TitleScreen();
	~TitleScreen();

	bool Load(Textures* tex);

	bool Update(Input* input, float dt);

	bool Draw(Render* render);

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool Unload();
private:

	SDL_Texture* titleTex = nullptr;
	SDL_Texture* guiTex = nullptr;
	SDL_Rect title;

	GuiButton* btnStart;
	GuiButton* btnContinue;
	GuiButton* btnSettings;
	GuiButton* btnCredits;
	GuiButton* btnExit;

	int textFont = -1;
};

#endif // __SCENE_H__