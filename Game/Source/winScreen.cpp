#include "WinScreen.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "TitleScreen.h"
#include "LevelScene.h"
#include "ModuleCollisions.h"

#include "Defs.h"
#include "Log.h"

WinScreen::WinScreen()
{
	name = "Initial S";

	logo = { 185, 0, 962, 720 };
}

WinScreen::~WinScreen() {}

bool WinScreen::Load(Textures* tex)
{
	bool ret = true;
	// Include logo
	app->audio->PlayMusic("Assets/Audio/music/win_soundtrack.ogg");

	logoTex = app->tex->Load("Assets/Screens/win_screen.png");
	app->render->background = { 0,0,0,0 };
	if (logoTex == nullptr)
	{
		ret = false;
	}

	app->render->camera = { 0,0,1280,720 };
	return ret;
}

bool WinScreen::Update(Input* input, float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		TransitionToScene(SceneType::TITLE);
	}
	return ret;
}

bool WinScreen::Draw(Render* render)
{
	bool ret = true;
	app->render->DrawTexture(logoTex, 0, 0);
	return ret;
}

bool WinScreen::OnGuiMouseClickEvent(GuiControl* control)
{
	return false;
}

bool WinScreen::Unload()
{
	bool ret = true;

	if (!app->tex->UnLoad(logoTex))
	{
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}

	return ret;
}