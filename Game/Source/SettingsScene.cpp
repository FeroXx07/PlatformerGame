#include "SettingsScene.h"
#include "Audio.h"
#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Textures.h"
#include "SceneManager.h"

SettingsScene::SettingsScene(bool fullScreen , bool vSync)
{
	name = "settings";
	settingsTexture = NULL;

	sliderMusicVolume = new GuiSlider(1, { 630, 116, 400, 67 }, "musicVolume");
	sliderMusicVolume->maxValue = SDL_MIX_MAXVOLUME;
	sliderMusicVolume->minValue = 0;
	sliderMusicVolume->SetObserver(this);

	sliderFxVolume = new GuiSlider(2, { 630, 241, 400, 67 }, "fxVolume");
	sliderFxVolume->maxValue = SDL_MIX_MAXVOLUME;
	sliderFxVolume->minValue = 0;
	sliderFxVolume->SetObserver(this);

	boxFullScreen = new GuiCheckBox(3, { 700, 352, 300, 103 }, "fullScreen");
	boxFullScreen->SetObserver(this);

	boxVsync = new GuiCheckBox(4, { 700, 479, 300, 103 }, "vSync");
	boxVsync->SetObserver(this);

	btnBack = new GuiButton(5, { 1280 / 2 - 300 / 2, 600, 394, 66 }, "back");
	btnBack->SetObserver(this);

	boxFullScreen->anim.PushBack({ 2765, 385, 273, 103 }); // Normal checked
	boxFullScreen->anim.PushBack({ 3039, 385, 301, 103 }); // Normal unchecked
	boxFullScreen->anim.PushBack({ 1906, 385, 273, 103 }); // Focused checked
	boxFullScreen->anim.PushBack({ 2180, 385, 301, 103 }); // Focused unchecked
	boxFullScreen->anim.PushBack({ 1142, 385, 273, 103 }); // Clicked checked
	boxFullScreen->anim.PushBack({ 1416, 385, 301, 103 }); // Clicked unchecked
	boxFullScreen->anim.loop = false;

	boxVsync->anim.PushBack({ 2765, 515, 323, 103 }); // Normal checked
	boxVsync->anim.PushBack({ 3089, 515, 321, 103 }); // Normal unchecked
	boxVsync->anim.PushBack({ 1906, 515, 323, 103 }); // Focused checked
	boxVsync->anim.PushBack({ 2230, 515, 321, 103 }); // Focused unchecked
	boxVsync->anim.PushBack({ 1142, 515, 323, 103 }); // Clicked checked
	boxVsync->anim.PushBack({ 1466, 515, 321, 103 }); // Clicked unchecked
	boxVsync->anim.loop = false;

	sliderMusicVolume->anim.PushBack({ 2783, 24, sliderMusicVolume->slider.w, sliderMusicVolume->slider.h });
	sliderMusicVolume->anim.PushBack({ 1923, 24, sliderMusicVolume->slider.w, sliderMusicVolume->slider.h });
	sliderMusicVolume->anim.PushBack({ 1155, 24, sliderMusicVolume->slider.w, sliderMusicVolume->slider.h });
	sliderMusicVolume->anim.loop = false;

	sliderFxVolume->anim.PushBack({ 2783, 24, sliderMusicVolume->slider.w, sliderMusicVolume->slider.h });
	sliderFxVolume->anim.PushBack({ 1923, 24, sliderMusicVolume->slider.w, sliderMusicVolume->slider.h });
	sliderFxVolume->anim.PushBack({ 1155, 24, sliderMusicVolume->slider.w, sliderMusicVolume->slider.h });
	sliderFxVolume->anim.loop = false;

	btnBack->anim.PushBack({ 2765, 289, 394, 66 });
	btnBack->anim.PushBack({ 1906, 289, 394, 66 });
	btnBack->anim.PushBack({ 1142, 289, 394, 66 });
	btnBack->anim.loop = false;

	this->fullScreen = fullScreen;
	this->vSync = vSync;
	this->musicVolume = app->audio->GetMusicVolume();
	this->fxVolume = app->audio->GetFxVolume();

	sliderMusicVolume->value = this->musicVolume;
	sliderFxVolume->value = this->fxVolume;

	if (this->fullScreen)
		boxFullScreen->checked = true;

	if (this->vSync)
		boxVsync->checked = true;
}

SettingsScene::~SettingsScene()
{
}

bool SettingsScene::Load(Textures* tex)
{
	bool ret = true;
	settingsTexture = tex->Load("Assets/Screens/settings_menu.png");
	btnBack->texture = sliderFxVolume->texture = sliderMusicVolume->texture = boxVsync->texture = boxFullScreen->texture = settingsTexture;
	vSync = app->vSync;

	sliderMusicVolume->fxClick = sliderFxVolume->fxClick = btnBack->fxClick = boxFullScreen->fxClick = boxVsync->fxClick = app->audio->LoadFx("Assets/Audio/Fx/click_mouse.wav");
	sliderMusicVolume->fxHover = sliderFxVolume->fxHover = btnBack->fxHover = boxFullScreen->fxHover = boxVsync->fxHover = app->audio->LoadFx("Assets/Audio/Fx/hover_mouse.wav");

	return ret;
}

bool SettingsScene::Update(Input* input, float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		app->debugGui = !app->debugGui;

	sliderMusicVolume->Update(input, dt);
	sliderFxVolume->Update(input, dt);
	boxFullScreen->Update(input, dt);
	btnBack->Update(input, dt);
	boxVsync->Update(input, dt);
	return ret;
}

bool SettingsScene::Draw(Render* render)
{
	bool ret = true;
	render->DrawRectangle({ 0,0,1280,720 }, 20, 103, 112, 255);
	SDL_Rect settingsBox = { 43, 24, 1077, 618 };
	render->DrawTexture(settingsTexture, 100, 0, &settingsBox, 0.0f);

	sliderMusicVolume->DrawTexture(render);
	sliderFxVolume->DrawTexture(render);
	boxFullScreen->DrawTexture(render);
	boxVsync->DrawTexture(render);
	btnBack->DrawTexture(render);

	if (app->debugGui)
	{
		sliderMusicVolume->DrawDebug(render);
		sliderFxVolume->DrawDebug(render);
		boxFullScreen->DrawDebug(render);
		btnBack->DrawDebug(render);
		boxVsync->DrawDebug(render);
	}
	return ret;
}

bool SettingsScene::OnGuiMouseClickEvent(GuiControl* control)
{
	bool ret = true;

	switch (control->type)
	{
	case GuiControlType::SLIDER:
	{
		GuiSlider* slider = (GuiSlider*)control;
		if (slider->id == 1)
		{
			// Music volume
			app->audio->ChangeMusicVolume(slider->value);
		}
		else if (slider->id == 2)
		{
			// Fx volume
			app->audio->ChangeFxVolume(slider->value);
		}
		break;
	}
	case GuiControlType::BUTTON:
	{
		if (control->id == 5)
		{
			TransitionToScene(SceneType::TITLE);
		}
		break;
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 3)
		{
			GuiCheckBox* checkBox = (GuiCheckBox*)control;
			if (checkBox->checked)
			{
				fullScreen = true;
				app->win->fullScreen = fullScreen;
				Uint32 flags = SDL_WINDOW_SHOWN;
				if (fullScreen == true) flags |= SDL_WINDOW_FULLSCREEN;

				Uint32 flags2 = SDL_RENDERER_ACCELERATED;
				if (vSync == true ) flags2 |= SDL_RENDERER_PRESENTVSYNC;

				SDL_DestroyWindow(app->win->window);
				app->win->window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, flags);

				SDL_DestroyRenderer(app->render->renderer);
				app->render->renderer = SDL_CreateRenderer(app->win->window, -1, flags2);
			}
			else
			{
				fullScreen = false;
				app->win->fullScreen = fullScreen;
				Uint32 flags = SDL_WINDOW_SHOWN;
				if (fullScreen == true) flags |= SDL_WINDOW_FULLSCREEN;

				Uint32 flags2 = SDL_RENDERER_ACCELERATED;
				if (vSync == true) flags2 |= SDL_RENDERER_PRESENTVSYNC;

				SDL_DestroyWindow(app->win->window);
				app->win->window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, flags);

				SDL_DestroyRenderer(app->render->renderer);
				app->render->renderer = SDL_CreateRenderer(app->win->window, -1, flags2);
			}
			this->TransitionToScene(SceneType::SETTINGS);
		}
		if (control->id == 4)
		{
			GuiCheckBox* checkBox = (GuiCheckBox*)control;
			if (checkBox->checked)
			{
				vSync = true;
				app->vSync = vSync;

				Uint32 flags = SDL_WINDOW_SHOWN;
				if (fullScreen == true) flags |= SDL_WINDOW_FULLSCREEN;

				Uint32 flags2 = SDL_RENDERER_ACCELERATED;
				if (vSync == true) flags2 |= SDL_RENDERER_PRESENTVSYNC;

				SDL_DestroyWindow(app->win->window);
				app->win->window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, flags);

				SDL_DestroyRenderer(app->render->renderer);
				app->render->renderer = SDL_CreateRenderer(app->win->window, -1, flags2);
			}
			else
			{
				vSync = false;
				app->vSync = vSync;

				Uint32 flags = SDL_WINDOW_SHOWN;
				if (fullScreen == true) flags |= SDL_WINDOW_FULLSCREEN;

				Uint32 flags2 = SDL_RENDERER_ACCELERATED;
				if (vSync == true) flags2 |= SDL_RENDERER_PRESENTVSYNC;

				SDL_DestroyWindow(app->win->window);
				app->win->window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, flags);

				SDL_DestroyRenderer(app->render->renderer);
				app->render->renderer = SDL_CreateRenderer(app->win->window, -1, flags2);
			}
			this->TransitionToScene(SceneType::SETTINGS);
		}
		break;
	}
	default: 
		break;
	}
	
	return ret;
}

bool SettingsScene::Unload()
{
	bool ret = true;

	if (settingsTexture != nullptr)
		app->tex->UnLoad(settingsTexture);

	RELEASE(sliderMusicVolume);
	RELEASE(sliderFxVolume);
	RELEASE(btnBack);
	delete sliderMusicVolume;
	delete sliderFxVolume;
	delete btnBack;

	return ret;
}
