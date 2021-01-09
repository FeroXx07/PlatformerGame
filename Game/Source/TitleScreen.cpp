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

TitleScreen::TitleScreen()
{
	name = "Title S";

	title = { 185,0,962,720 };

	btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 390, 55 }, "start");
	btnStart->SetObserver(this);

	btnContinue = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 390, 55 }, "continue");
	btnContinue->SetObserver(this);

	btnSettings = new GuiButton(3, { 1280 / 2 - 300 / 2, 500, 390, 73 }, "settings");
	btnSettings->SetObserver(this);

	btnCredits = new GuiButton(4, { 1280 / 2 - 300 / 2, 600, 390, 55 }, "credits");
	btnCredits->SetObserver(this);

	btnExit = new GuiButton(5, { 0, 600, 390, 57 }, "exit");
	btnExit->SetObserver(this);

	btnStart->anim.PushBack({ 83,66,389,55 }); // Normal 0
	btnStart->anim.PushBack({ 586,66,389,55 }); // Focused 1
	btnStart->anim.PushBack({ 1699,66,389,55 }); // Pressed 2

	btnContinue->anim.PushBack({ 83,161,389,55 }); // Normal 0
	btnContinue->anim.PushBack({ 586,161,389,55 }); // Focused 1
	btnContinue->anim.PushBack({ 1699,161,389,55 }); // Pressed 2

	btnSettings->anim.PushBack({ 83,257,389,73 }); // Normal 0
	btnSettings->anim.PushBack({ 586,257,389,73 }); // Focused 1
	btnSettings->anim.PushBack({ 1699,257,389,73 }); // Pressed 2

	btnCredits->anim.PushBack({ 83,351,389,73 }); // Normal 0
	btnCredits->anim.PushBack({ 586,351,389,73 }); // Focused 1
	btnCredits->anim.PushBack({ 1699,351,389,73 }); // Pressed 2

	btnExit->anim.PushBack({ 83,449,389,57 }); // Normal 0
	btnExit->anim.PushBack({ 586,449,389,57 }); // Focused 1
	btnExit->anim.PushBack({ 1699,449,389,57 }); // Pressed 2
}

TitleScreen::~TitleScreen() {}

bool TitleScreen::Load(Textures* tex)
{
	bool ret = true;

	// Include title
	app->audio->PlayMusic("Assets/Audio/music/initial_screen.ogg");
	titleTex = app->tex->Load("Assets/Screens/title_screen.png");

	btnExit->texture = btnCredits->texture= btnSettings->texture = btnContinue->texture = btnStart->texture = guiTex = app->tex->Load("Assets/Screens/title_screen_main_menu.png");
	btnExit->fxClick = btnCredits->fxClick = btnSettings->fxClick = btnContinue->fxClick = btnStart->fxClick = app->audio->LoadFx("Assets/Audio/Fx/click_mouse.wav");
	btnExit->fxHover = btnCredits->fxHover = btnSettings->fxHover = btnContinue->fxHover = btnStart->fxHover = app->audio->LoadFx("Assets/Audio/Fx/hover_mouse.wav");
	//app->render->background = { 0,0,0,0 };

	return ret;
}

bool TitleScreen::Update(Input* input, float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		app->debugGui = !app->debugGui;

	btnStart->Update(input, dt);
	btnContinue->Update(input, dt);
	btnSettings->Update(input, dt);
	btnCredits->Update(input, dt);
	btnExit->Update(input, dt);
	return ret;
}

bool TitleScreen::Draw(Render* render)
{
	bool ret = true;

	app->render->DrawTexture(titleTex, 0, 0,NULL, 0.0f);

	btnStart->DrawTexture(render);
	btnContinue->DrawTexture(render);
	btnSettings->DrawTexture(render);
	btnCredits->DrawTexture(render);
	btnExit->DrawTexture(render);

	if (app->debugGui)
	{
		btnStart->DrawDebug(render);
		btnContinue->DrawDebug(render);
		btnSettings->DrawDebug(render);
		btnCredits->DrawDebug(render);
		btnExit->DrawDebug(render);
	}
	return ret;
}

bool TitleScreen::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) TransitionToScene(SceneType::GAMEPLAY);
		else if (control->id == 2) TransitionToScene(SceneType::LOAD_GAMEPLAY);
		else if (control->id == 3) TransitionToScene(SceneType::SETTINGS);
		else if (control->id == 4) TransitionToScene(SceneType::CREDITS);// Transition to credits scene
		else if (control->id == 5) app->input->CloseApp();// TODO: Exit request
	}
	default: break;
	}

	return true;
}

bool TitleScreen::Unload()
{
	bool ret = true;

	if (!app->tex->UnLoad(titleTex))
	{
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}

	if (!app->tex->UnLoad(guiTex))
	{
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}

	RELEASE(btnContinue);
	RELEASE(btnCredits);
	RELEASE(btnSettings);
	RELEASE(btnStart);
	delete btnContinue;
	delete btnCredits;
	delete btnSettings;
	delete btnStart;

	return ret;
}
