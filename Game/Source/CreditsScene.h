#include "SDL/include/SDL_rect.h"

#include "Module.h"
#include "Textures.h"
#include "Scene.h"
#include "GuiButton.h"

struct SDL_Rect;

class CreditsScene : public Scene
{
public:

	CreditsScene();
	~CreditsScene();

	bool Load(Textures* tex);

	bool Update(Input* input, float dt);

	bool Draw(Render* render);

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool Unload();
private:

	SDL_Texture* creditsTex = nullptr;
	SDL_Texture* guiTex = nullptr;
	GuiButton* btnBack;
};