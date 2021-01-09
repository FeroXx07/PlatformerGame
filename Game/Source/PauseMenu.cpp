#include "PauseMenu.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"

PauseScene::PauseScene()
{
	btnResume = new GuiButton(1, { 1280 / 2 - 300 / 2 + 55, 250, 162, 55 }, "resume");
	btnResume->SetObserver(this);

	btnSettings = new GuiButton(2, { 1280 / 2 - 300 / 2 + 55, 300 + 20, 162, 55 }, "settings");
	btnSettings->SetObserver(this);

	btnBack = new GuiButton(3, { 1280 / 2 - 300 / 2, 350 + 40, 292, 46 }, "back");
	btnBack->SetObserver(this);

	btnExit = new GuiButton(4, { 1280 / 2 - 300 / 2 + 55, 400 + 60, 162, 55 }, "exit");
	btnExit->SetObserver(this);

	btnResume->anim.PushBack({ 928, 108, 158, 52 });
	btnResume->anim.PushBack({ 928, 31, 158, 52 });
	btnResume->anim.PushBack({ 717, 31, 158, 52 });
	btnResume->anim.loop = false;

	btnSettings->anim.PushBack({ 274, 447, 162, 52 });
	btnSettings->anim.PushBack({ 274, 585, 162, 52 });
	btnSettings->anim.PushBack({ 274, 516, 162, 52 });
	btnSettings->anim.loop = false;

	btnBack->anim.PushBack({ 716, 191, 292, 46 });
	btnBack->anim.PushBack({ 716, 327, 292, 46 });
	btnBack->anim.PushBack({ 716, 254, 292, 46 });
	btnBack->anim.loop = false;

	btnExit->anim.PushBack({ 29, 447, 162, 52 });
	btnExit->anim.PushBack({ 29, 520, 162, 52 });
	btnExit->anim.PushBack({ 29, 589, 162, 52 });
	btnExit->anim.loop = false;
}

PauseScene::~PauseScene()
{
}

bool PauseScene::Load(Textures* tex)
{
	//app->render->background = { 0,0,0,0 };
	pauseTexture = app->tex->Load("Assets/Screens/pause_menu.png");
	btnResume->texture = btnSettings->texture = btnBack->texture = btnExit->texture = pauseTexture;

	btnResume->fxClick = btnSettings->fxClick = btnBack->fxClick = btnExit->fxClick = app->audio->LoadFx("Assets/Audio/Fx/click_mouse.wav");
	btnResume->fxHover = btnSettings->fxHover = btnBack->fxHover = btnExit->fxHover = app->audio->LoadFx("Assets/Audio/Fx/hover_mouse.wav");

	return false;
}

bool PauseScene::Update(Input* input, float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		app->debugGui = !app->debugGui;

	btnResume->Update(input, dt);
	btnSettings->Update(input, dt);
	btnBack->Update(input, dt);
	btnExit->Update(input, dt);
	return false;
}

bool PauseScene::Draw(Render* render)
{
	SDL_Rect pauseBox = { 24, 26, 661, 404 };
	render->DrawTexture(pauseTexture, 300, 150, &pauseBox, 0.0f);

	btnResume->DrawTexture(render);
	btnSettings->DrawTexture(render);
	btnBack->DrawTexture(render);
	btnExit->DrawTexture(render);

	if (app->debugGui)
	{
		btnResume->DrawDebug(render);
		btnSettings->DrawDebug(render);
		btnBack->DrawDebug(render);
		btnExit->DrawDebug(render);
	}

	return false;
}

bool PauseScene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) TransitionToScene(SceneType::LOAD_GAMEPLAY);
		else if (control->id == 2) TransitionToScene(SceneType::SETTINGS);
		else if (control->id == 3) TransitionToScene(SceneType::TITLE);
		else if (control->id == 4) app->input->CloseApp();
	}
	default: break;
	}
	return false;
}

bool PauseScene::Unload()
{
	app->tex->UnLoad(pauseTexture);

	RELEASE(btnBack);
	RELEASE(btnExit);
	RELEASE(btnResume);
	RELEASE(btnSettings);
	delete btnBack;
	delete btnExit;
	delete btnResume;
	delete btnSettings;
	return false;
}
