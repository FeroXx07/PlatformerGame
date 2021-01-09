#include "ModuleInitialScreen.h"
#include "App.h"
#include "Textures.h"
#include "ModuleFadeToBlack.h"
#include "Input.h"
#include "TitleScreen.h"
#include "Window.h"
#include "Render.h"
#include "Audio.h"
#include "Log.h"

#define LOGO_FADE_SPEED 1.0f

ModuleInitialScreen::ModuleInitialScreen()
{
	name = "Initial S";
	logo = { 185, 0, 962, 720 };
	logoTex = tex = nullptr;

	state = 0;
	timeCounter = 0.0f;
	logoAlpha = 1.0f;
}

ModuleInitialScreen::~ModuleInitialScreen()
{
}

bool ModuleInitialScreen::Load(Textures* tex)
{
	bool ret = true;

	// Include logo
	logoTex = app->tex->Load("Assets/Screens/logo_screen.png");
	app->render->background = { 0,0,0,0 };
	if (logoTex == nullptr)
	{
		ret = false;
	}

	return ret;
}

bool ModuleInitialScreen::Update(Input* input, float dt)
{
	bool ret = true;

	if (state == 0)
	{
		state = 1;
	}
	else if (state == 1)
	{
		logoAlpha -= (LOGO_FADE_SPEED * dt);

		if (logoAlpha <= 0.0f)
		{
			logoAlpha = 0.0f;
			state = 2;
		}
	}
	else if (state == 2)
	{
		// Waiting for 3 seconds
		timeCounter += dt;
		if (timeCounter >= 3.0f) state = 3;
	}
	else if (state == 3)
	{
		logoAlpha += (LOGO_FADE_SPEED * dt);

		if (logoAlpha >= 1.0f)
		{
			logoAlpha = 1.0f;
			TransitionToScene(SceneType::TITLE);
		}
	}

	return ret;
}

bool ModuleInitialScreen::Draw(Render* render)
{
	bool ret = true;
	
	render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, 255 }); // Black background
	render->DrawTexture(logoTex, 0, 0); // Logo texture
	render->DrawRectangle(logo, { 0, 0, 0, (uchar)(255.0f * logoAlpha) }); // Changing alpha rect

	return ret;
}

bool ModuleInitialScreen::Unload()
{
	bool ret = true;

	if (!app->tex->UnLoad(logoTex))
	{
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}

	return ret;
}


