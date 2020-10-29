#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "ModuleFadeToBlack.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"
#include "ModulePlayer.h"

Scene::Scene() : Module(true) // The argument passed to the parent constructor is if it is enabled at construction
{
	name.Create("scene");
}

Scene::Scene(bool b) : Module(b) // The argument passed to the parent constructor is if it is enabled at construction
{
	name.Create("scene");
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
	img = app->tex->Load("Assets/textures/01.png");
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	app->map->Load("01Resize.tmx");
	app->render->SetBackgroundColor(app->map->data.backgroundColor);
	// Layers gets gid correctly

	app->map->LoadColliders();

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
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 2;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 2;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 2;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 2;
		
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_UP)
		app->fade->FadeToBlack(this, (Module*)app->scene2);

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		app->fade->FadeToBlack(this, (Module*)app->deathScene);

	//printf("Camera X: %d, \n Camera Y: %d\n\n", app->render->camera.x, app->render->camera.y);
	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more


	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->data.width, app->map->data.height,
				   app->map->data.tileWidth, app->map->data.tileHeight,
				   app->map->data.tilesets.count());

	app->win->SetTitle(title.GetString());
	iPoint mapPos = app->map->WorldToMap(app->player->playerPos.x, app->player->playerPos.y);
	printf("Position in MAP X = %d\nPosition in MAP Y = %d\n\n", mapPos.x,mapPos.y);
	

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	// Draw map
	app->map->Draw();

	//app->render->DrawTexture(img, 0, 0);


	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(img);
	
	return true;
}
