#include "ModuleInitialScreen.h"
#include "App.h"
#include "Textures.h"
#include "ModuleFadeToBlack.h"
#include "Input.h"
#include "Scene.h"
#include "ModuleFonts.h"
#include "Render.h"
#include "Audio.h"

ModuleInitialScreen::ModuleInitialScreen(bool startEnabled) : Module(startEnabled)
{
	name = "Initial S";

	logo = { 0, 0,256,224 };
	screen = { 0,0,SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };
}

ModuleInitialScreen::~ModuleInitialScreen() {}

bool ModuleInitialScreen::Start()
{
	bool ret = true;

	startTime = SDL_GetTicks();
	actualTime = 0;
	endTime = 3000;

	// Include logo

	if (tex == nullptr)
	{
		ret = false;
	}

	return ret;
}

update_status ModuleInitialScreen::Update()
{
	update_status ret = update_status::UPDATE_CONTINUE;

	if (app->input->GetKey[SDL_SCANCODE_RETURN] == KEY_DOWN)
	{
		app->fade->FadeToBlack(this, (Module*)app->initialScreen);
	}

	return ret;
}

update_status ModuleInitialScreen::postUpdate()
{
	update_status ret = update_status::UPDATE_CONTINUE;

	actualTime = SDL_GetTicks() - startTime;

	return ret;
}

bool ModuleInitialScreen::CleanUp()
{
	bool ret = true;

	startTime = 0;
	endTime = 0;
	actualTime = 0;

	if (!app->tex->UnLoad(tex))
	{
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}

	if (!app->tex->UnLoad(tex))
	{
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}

	return ret;
}