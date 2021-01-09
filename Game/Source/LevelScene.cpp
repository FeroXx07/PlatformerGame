#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "LevelScene.h"
#include "Map.h"
#include "ModuleCollisions.h"
#include "EntityManager.h"
#include "Player.h"
#include "Pathfinding.h"
#include "SceneManager.h"
#include "Timer.h"

#include "Defs.h"
#include "Log.h"

LevelScene::LevelScene() : Scene() // The argument passed to the parent constructor is if it is enabled at construction
{
	name.Create("scene");
	img = bg = NULL;
}

LevelScene::LevelScene(bool load) : Scene() // The argument passed to the parent constructor is if it is enabled at construction
{
	name.Create("scene");
	img = bg = NULL;

	toLoad = load;
}

// Destructor
LevelScene::~LevelScene()
{}

bool LevelScene::Load(Textures* tex)
{
	app->sceneManager->mapScene = map = new Map(app->tex);
	map->Load("level_one.tmx");
	{
		int w, h;
		uchar* data = NULL;

		if (map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	app->render->background = { 99,210,222,0 };
	app->audio->PlayMusic("Assets/audio/music/level_soundtrack.ogg");
	// Layers gets gid correctly

	if (app->collisions->IsEnabled() == false)
		app->collisions->Enable();

	if (app->entityman->IsEnabled() == false)
		app->entityman->Enable();

	map->LoadColliders();
	app->sceneManager->playerScene = player = (Player*)app->entityman->CreateEntity(EntityType::PLAYER, 30 * 32, 9 * 32);
	app->entityman->CreateEntity(EntityType::HUD, 30 * 32, 9 * 32);
	if (toLoad == false)
	{
		player->destroyed = false;
		player->win = false;
		player->position = { 30 * 32, 9 * 32 };

		player->velocity.y = 0;
		player->cameraFollow = true;
		player->lives = 3;
		player->health = 3;

		app->entityman->CreateEntity(EntityType::ITEM_HEALTH, -100, -100);

		app->entityman->CreateEntity(EntityType::ITEM_STAR, 71 * 32, 7 * 32);
		app->entityman->CreateEntity(EntityType::ITEM_STAR, 114 * 32, 8 * 32);
		app->entityman->CreateEntity(EntityType::ITEM_STAR, 224 * 32, 15 * 32);
		app->entityman->CreateEntity(EntityType::ITEM_HEALTH, 168 * 32, 10 * 32);
		app->entityman->CreateEntity(EntityType::ITEM_HEALTH, 255 * 32, 11 * 32);

		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 36 * 32, 448 + 20);
		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 20 * 32, 448 + 20);
		app->entityman->CreateEntity(EntityType::ENEMY_FLYING, 15 * 32, 448 + 20);

		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 75 * 32, 11 * 32 - 12);
		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 90 * 32, 7 * 32 - 12);
		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 90 * 32, 15 * 32 - 12);

		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 122 * 32, 7 * 32 - 12);
		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 126 * 32, 7 * 32 - 12);

		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 155 * 32, 15 * 32 - 12);

		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 166 * 32, 12 * 32 - 12);
		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 174 * 32, 12 * 32 - 12);

		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 194 * 32, 15 * 32 - 12);

		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 214 * 32, 14 * 32 - 12);

		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 220 * 32, 16 * 32 - 12);
		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 235 * 32, 16 * 32 - 12);

		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 250 * 32, 15 * 32 - 12);

		app->entityman->CreateEntity(EntityType::ENEMY_WALKING, 264 * 32, 18 * 32 - 12);


		app->entityman->CreateEntity(EntityType::ENEMY_FLYING, 60 * 32, 12 * 32);
		//app->entityman->CreateEntity(EntityType::ENEMY_FLYING, 90 * 32, 11 * 32);
		///*app->entityman->CreateEntity(EntityType::ENEMY_FLYING, 101 * 32, 11 * 32);
		//app->entityman->CreateEntity(EntityType::ENEMY_FLYING, 142 * 32, 8 * 32);
		//app->entityman->CreateEntity(EntityType::ENEMY_FLYING, 170 * 32, 7 * 32);
		//app->entityman->CreateEntity(EntityType::ENEMY_FLYING, 220 * 32, 10 * 32);
		//app->entityman->CreateEntity(EntityType::ENEMY_FLYING, 265 * 32, 14 * 32);*/
	}
	else
	{
		app->LoadGameRequest();
	}

	resetCounter = 0;

	app->SaveGameRequest();

	font = new Font("Assets/Fonts/potta_one_regular.xml", app->tex);

	timerSec.Start();
	return true;
}

bool LevelScene::Update(Input* input, float dt)
{
	// DEBUG KEYS
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->SaveGameRequest();
		TransitionToScene(SceneType::PAUSE);
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		TransitionToScene(SceneType::GAMEPLAY);

	// L02: DONE 3: Request Load / Save when pressing L/S
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 2;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 2;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 2;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 2;
	//	
	//if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_UP || app->input->GetKey(SDL_SCANCODE_F3) == KEY_UP)
	//	app->fade->FadeToBlack(this, (Module*)app->scene);

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN || player->destroyed == true)
	{
		//Check here if player has lost all lives, if true then do the fade to black (permanent deathAnim)
		if (player->lives > 0)
		{
			//If player still has lives, minus one live and restart him from the last checkpoint (temporal deathAnim)
			player->PlayerDied();
		}
		else if (player->lives == 0)
		{
			if (resetCounter == 3 * 60)
			{
				//app->fade->FadeToBlack(this, (Module*)app->deathScene);
				resetCounter = 0;
			}
			++resetCounter;
		}

	}
	/*printf("lives = %u\n", app->player->lives);
	printf("health = %u\n", app->player->health);*/

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_UP || player->win == true)
		TransitionToScene(SceneType::WIN);

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		map->data.width, map->data.height,
		map->data.tileWidth, map->data.tileHeight,
		map->data.tilesets.Count());

	//app->win->SetTitle(title.GetString());
	iPoint mapPos = map->WorldToMap(player->position.x, player->position.y);
	//printf("Position in MAP X = %d\nPosition in MAP Y = %d\n\n", mapPos.x,mapPos.y);

	if (player->win == true)
	{
		TransitionToScene(SceneType::WIN);
	}
	if (player->lives == 0)
	{
		TransitionToScene(SceneType::DEATH);
	}

	if (timerSec.ReadSec() >= 5.0f && timerSec.ReadSec() <= 6.0f)
	{
		app->SaveGameRequest();
	}
	return true;
}

bool LevelScene::Draw(Render* render)
{
	bool ret = true;

	int time = timerSec.ReadSec();

	sprintf_s(counterText, 5, "%0d", time);
	app->render->DrawText(font, "Timer:", 10, 0, 50, 0, { 255, 165, 0, 255 });
	app->render->DrawText(font, counterText, 120, 0, 50, 0, { 255, 165, 0, 255 });

	map->Draw(app->render);
	return true;
}

bool LevelScene::Unload()
{
	app->tex->UnLoad(bg);
	app->collisions->Disable();
	app->entityman->Disable();
	map->CleanUp();
	RELEASE(map);
	delete map;
	SDL_ShowCursor(SDL_ENABLE);
	return true;
}



