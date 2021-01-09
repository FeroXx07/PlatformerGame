#include "SceneManager.h"
#include "App.h"

#include "ModuleInitialScreen.h"
#include "TitleScreen.h"
#include "Scene.h"
#include "WinScreen.h"
#include "DeathScene.h"
#include "LevelScene.h"
#include "SettingsScene.h"
#include "PauseMenu.h"
#include "WinScreen.h"
#include "DeathScene.h"
#include "CreditsScene.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Window.h"
//#include "GuiButton.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager(Input* input, Render* render, Textures* tex) : Module()
{
	name.Create("scenemanager");

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;;

	this->input = input;
	this->render = render;
	this->tex = tex;
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake()
{
	LOG("Loading Scene manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	//previousScene = new ModuleInitialScreen();
	current = new ModuleInitialScreen();

	current->previousScene = SceneType::LOGO;
	current->currentScene = SceneType::LOGO;

	current->Load(tex);

	next = nullptr;

	// For volume getters purposes
	app->audio->LoadFx("Assets/Audio/Fx/player_shot.wav");
	return true;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	if (!onTransition)
	{
		//if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) render->camera.y -= 1;
		//if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) render->camera.y += 1;
		//if (input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) render->camera.x -= 1;
		//if (input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) render->camera.x += 1;

		current->Update(input, dt);
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f)
			{
				transitionAlpha = 1.0f;

				SceneType actual;
				actual = current->nextScene;

				current->Unload();	// Unload previousScene screen
				next->Load(tex);	// Load next screen

				RELEASE(current);	// Free previousScene pointer
				current = next;		// Assign next pointer
				next = nullptr;

				current->currentScene = actual;
				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;
			}
		}
		else  // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	// DrawDebug previousScene scene
	current->Draw(render);

	// DrawDebug full screen rectangle in front of everything
	if (onTransition)
	{
		render->DrawRectangle({ 0, 0, 1280, 720 },  0, 0, 0, (unsigned char)(255.0f * transitionAlpha),true,false);
	}

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
			//case SceneType::LOGO: next = new SceneLogo(); break;
			case SceneType::TITLE: 
			{
				next = new TitleScreen();
				break;
			}
			//case SceneType::GAMEPLAY: next = new SceneGameplay(); break;
			case SceneType::SETTINGS: 
			{
				next = new SettingsScene(app->win->fullScreen, app->vSync);
				break;
			}
			case SceneType::GAMEPLAY: 
			{
				next = new LevelScene(false);
				break;
			}
			case SceneType::LOAD_GAMEPLAY: 
			{
				next = new LevelScene(true);
				break;
			}
			case SceneType::PAUSE:
			{
				next = new PauseScene();
				break;
			}
			case SceneType::WIN:
			{
				next = new WinScreen();
				break;
			}
			case SceneType::DEATH:
			{
				next = new DeathScene();
				break;
			}
			case SceneType::CREDITS:
			{
				next = new CreditsScene();
				break;
			}
			default: break;
		}
		current->previousScene = current->currentScene;
		current->currentScene = current->nextScene;
		current->transitionRequired = false;
	}

	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing scene");

	if (current != nullptr) 

		current->Unload();

	return true;
}
