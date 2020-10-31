#include "winScreen.h"
#include "App.h"
#include "Textures.h"
#include "ModuleFadeToBlack.h"
#include "Input.h"
#include "Scene2.h"
#include "ModuleFonts.h"
#include "Render.h"
#include "Audio.h"
#include "ModulePlayer.h"

WinScreen::WinScreen(bool b) : Module(b)
{
	name = "Initial S";

	logo = { 185, 0, 962, 720 };
	screen = { 0,0,SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };
}

WinScreen::~WinScreen() {}

bool WinScreen::Start()
{
	bool ret = true;

	startTime = SDL_GetTicks();
	actualTime = 0;
	endTime = 3000;

	// Include logo

	logoTex = app->tex->Load("Assets/textures/winScreen.png");
	app->render->background = { 0,0,0,0 };
	if (logoTex == nullptr)
	{
		ret = false;
	}

	app->render->camera = { 0,0,1280,720 };
	app->player->cameraFollow = false;
	return ret;
}

bool WinScreen::Update(float dt)
{
	bool ret = true;



	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fade->FadeToBlack(this, (Module*)app->titleScreen);
	}

	return ret;
}

bool WinScreen::PostUpdate()
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	actualTime = SDL_GetTicks() - startTime;

	if (actualTime < endTime)
	{

	}
	app->render->DrawTexture(logoTex, 0, 0);
	return ret;
}

bool WinScreen::CleanUp()
{
	bool ret = true;

	startTime = 0;
	endTime = 0;
	actualTime = 0;

	if (!app->tex->UnLoad(logoTex))
	{
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}

	return ret;
}