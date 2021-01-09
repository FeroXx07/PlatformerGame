#include "Scene.h"

#include "GuiButton.h"

struct SDL_Rect;

class PauseScene : public Scene
{
public:
	PauseScene();
	~PauseScene();

	bool Load(Textures* tex);

	bool Update(Input* input, float dt);

	bool Draw(Render* render);

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool Unload();
private:

	SDL_Texture* pauseTexture = nullptr;
	SDL_Rect title;

	GuiButton* btnResume;
	GuiButton* btnSettings;
	GuiButton* btnBack;
	GuiButton* btnExit;
};
