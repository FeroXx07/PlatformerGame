#include "CreditsScene.h"

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

CreditsScene::CreditsScene()
{
	btnBack = new GuiButton(5, { 1280 / 2 - 300 / 2, 600, 394, 66 }, "back");
	btnBack->SetObserver(this);

	btnBack->anim.PushBack({ 2765, 289, 394, 66 });
	btnBack->anim.PushBack({ 1906, 289, 394, 66 });
	btnBack->anim.PushBack({ 1142, 289, 394, 66 });
	btnBack->anim.loop = false;
}

CreditsScene::~CreditsScene()
{
}

bool CreditsScene::Load(Textures* tex)
{
	creditsTex = tex->Load("Assets/Screens/credits.png");
	btnBack->texture = guiTex = tex->Load("Assets/Screens/settings_menu.png");
	btnBack->fxClick = app->audio->LoadFx("Assets/Audio/Fx/click_mouse.wav");
	btnBack->fxHover = app->audio->LoadFx("Assets/Audio/Fx/hover_mouse.wav");

	return false;
}

bool CreditsScene::Update(Input* input, float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		app->debugGui = !app->debugGui;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		TransitionToScene(SceneType::TITLE);
	}

	btnBack->Update(input, dt);
	return false;
}

bool CreditsScene::Draw(Render* render)
{
	render->DrawTexture(creditsTex, 0, 0);

	btnBack->DrawTexture(render);

	if (app->debugGui)
	{
		btnBack->DrawDebug(render);
	}

	return false;
}

bool CreditsScene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 5) // TODO: Exit request
		{
			TransitionToScene(SceneType::TITLE);
		}
	}

	}
	return false;
}

bool CreditsScene::Unload()
{
	app->tex->UnLoad(creditsTex);
	app->tex->UnLoad(guiTex);
	return false;
}
