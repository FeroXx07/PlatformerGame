#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "ModuleFadeToBlack.h"
#include "Map.h"
#include "ModuleCollisions.h"
#include "ModulePlayer.h"
#include "ModuleEntities.h"

#include "Defs.h"
#include "Log.h"
#include "ModulePlayer.h"

Scene::Scene() : Module(true) // The argument passed to the parent constructor is if it is enabled at construction
{
	name.Create("scene");
	img = bg = NULL;
}

Scene::Scene(bool b) : Module(b) // The argument passed to the parent constructor is if it is enabled at construction
{
	name.Create("scene");
	img = bg = NULL;

}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	app->map->Load("Level1.tmx");

	app->render->background = { 99,210,222,0 };
	// Layers gets gid correctly
	
	if (app->collisions->IsEnabled() == false)
		app->collisions->Enable();

	if (app->player->IsEnabled() == false)
		app->player->Enable();

	if (app->entities->IsEnabled() == false)
		app->entities->Enable();

	app->map->LoadColliders();

	app->player->destroyed = false;
	app->player->win = false;
	app->player->playerPos = { 30*32, 9*32 };
	
	app->player->velocity.y = 0;
	app->player->cameraFollow = true;


	app->entities->AddEnemy(EntityType::ENEMY_FIREMINION, 38 * 32, 9 * 32);
	resetCounter = 0;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 2;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 2;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 2;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 2;
		
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_UP || app->input->GetKey(SDL_SCANCODE_F3) == KEY_UP)
		app->fade->FadeToBlack(this, (Module*)app->scene);

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN || app->player->destroyed == true)
	{
		if (resetCounter == 3 * 60)
		{
			app->fade->FadeToBlack(this, (Module*)app->deathScene);
			resetCounter = 0;
		}
		++resetCounter;
	}

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_UP || app->player->win == true)
		app->fade->FadeToBlack(this, (Module*)app->winScreen);

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->data.width, app->map->data.height,
				   app->map->data.tileWidth, app->map->data.tileHeight,
				   app->map->data.tilesets.Count());

	//app->win->SetTitle(title.GetString());
	iPoint mapPos = app->map->WorldToMap(app->player->playerPos.x, app->player->playerPos.y);
	//printf("Position in MAP X = %d\nPosition in MAP Y = %d\n\n", mapPos.x,mapPos.y);
	

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->map->Draw();
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(bg);
	app->player->Disable();
	app->collisions->Disable();
	app->entities->Disable();
	app->map->CleanUp();
	return true;
}
