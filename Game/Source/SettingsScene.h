#include "Scene.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

struct SDL_Rect;

class SettingsScene : public Scene
{
public:
	SettingsScene(bool fullScreen = false, bool vSync = false);
	~SettingsScene();

	bool Load(Textures* tex);

	bool Update(Input* input, float dt);

	bool Draw(Render* render);

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool Unload();
private:

	SDL_Texture* settingsTexture = nullptr;
	SDL_Rect title;

	GuiSlider* sliderMusicVolume;
	GuiSlider* sliderFxVolume;
	GuiButton* btnBack;
	GuiCheckBox* boxFullScreen;
	GuiCheckBox* boxVsync;

public:
	bool fullScreen;
	bool vSync;
	int musicVolume;
	int fxVolume;
};