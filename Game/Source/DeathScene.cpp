#include "DeathScene.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "TitleScreen.h"
#include "ModuleFadeToBlack.h"
#include "LevelScene.h"
#include "ModuleCollisions.h"

#include "Defs.h"
#include "Log.h"

DeathScene::DeathScene()
{
	name = "Death S";

	logo = { 185, 0, 962, 720 };
}

DeathScene::~DeathScene() {}

bool DeathScene::Load(Textures* tex)
{
	bool ret = true;
	app->render->background = { 0,0,0,0 };
	// Include logo
	app->render->camera = { 0,0,1280,720 };
	logoTex = app->tex->Load("Assets/Screens/death_screen.png");

	app->audio->PlayMusic("Assets/Audio/music/loose_soundtrack.ogg");

	if (logoTex == nullptr)
	{
		ret = false;
	}

	app->render->camera = { 0,0,1280,720 };

	return ret;
}

bool DeathScene::Update(Input* input, float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		TransitionToScene(SceneType::TITLE);
	}

	return ret;
}

bool DeathScene::OnGuiMouseClickEvent(GuiControl* control)
{
	return false;
}

bool DeathScene::Draw(Render* render)
{
	bool ret = true;
	app->render->DrawTexture(logoTex, 0,0);
	return ret;
}

bool DeathScene::Unload()
{
	bool ret = true;

	if (!app->tex->UnLoad(logoTex))
	{
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}

	return ret;
}