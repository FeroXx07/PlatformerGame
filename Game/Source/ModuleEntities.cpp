#include "ModuleEntities.h"

#include "App.h"

#include "Render.h"
#include "Window.h"
#include "Textures.h"
#include "Audio.h"
#include "Log.h"
#include "List.h"
#include "ModuleCollisions.h"

#include "Entity.h"

#include "Enemy_FireMinion.h"
//#include "Item_Nut.h"
//#include "Item_Iron.h"
//#include "Item_Umbrella.h"
//#include "Item_Bag.h"
//#include "Item_Hammer.h"
//#include "Enemy_Minecart.h"


#define SPAWN_MARGIN 50


Entities::Entities(bool startEnabled) : Module(startEnabled)
{
	/*for(uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;*/
	
}

Entities::~Entities()
{

}

bool Entities::Start()
{
	/*texture = app->tex->Load("Assets/Background2.png");
	
	enemyDestroyedFx = app->audio->LoadFx("Assets/Music/SFX_Kill.wav");
	
	itemPickedFx = app->audio->LoadFx("Assets/Music/SFX_Bonus.wav");*/
	texture = app->tex->Load("Assets/textures/aa.png");


	return true;
}


bool Entities::PreUpdate()
{
	bool ret = true;
	// Remove all enemies scheduled for deletion
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->pendingToDelete)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}*/
	ListItem<Entity*>* list;
	list = entities.start;

	for (int i = 0; i < entities.Count(); ++i)
	{
		if (list != NULL && list->data->pendingToDelete)
		{
			/*delete newEntity;
			enemies[i] = nullptr;*/
			entities.Del(list);
		}
		list = list->next;
	}

	return ret;
}

bool Entities::Update(float dt)
{
	bool ret = true;
	HandleEnemiesSpawn();

	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
			enemies[i]->Update();
	}*/

	ListItem<Entity*>* list;
	list = entities.start;

	for (int i = 0; i < entities.Count(); ++i)
	{
		if (list != NULL)
			list->data->Update();
		list = list->next;
	}

	//HandleEnemiesDespawn();

	return ret;
}

bool Entities::PostUpdate()
{
	bool ret = true;
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
			enemies[i]->Draw();
	}*/

	ListItem<Entity*>* list;
	list = entities.start;

	for (int i = 0; i < entities.Count(); ++i)
	{
		if (list != NULL)
			list->data->Draw();
		list = list->next;
	}

	return ret;
}

// Called before quitting
bool Entities::CleanUp()
{
	bool ret = true;
	//app->audio->UnloadFx(enemyDestroyedFx);
	//
	//app->audio->UnloadFx(itemPickedFx);
	//
	//app->tex->Unload(texture);
	//

	LOG("Freeing all enemies");

	/*for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}*/

	ListItem<Entity*>* list;
	list = entities.start;
	for (int i = 0; i < entities.Count(); ++i)
	{
		if (list != NULL)
		{
			entities.Del(list);
		}
		list = list->next;
	}
	entities.Clear();

	return ret;
}

bool Entities::AddEntity(EntityType type, int x, int y)
{
	bool ret = true;

	EntitySpawnpoint spawn;
	spawn.type = type;
	spawn.x = x;
	spawn.y = y;

	spawnQueue.Add(spawn);

	return ret;
}

void Entities::HandleEnemiesSpawn()
{
	// Iterate all the enemies queue
	//for (uint i = 0; i < MAX_ENEMIES; ++i)
	//{
	//	if (spawnQueue[i].type != EntityType::NO_TYPE)
	//	{
	//		SpawnEnemy(spawnQueue[i]);
	//		spawnQueue[i].type = EntityType::NO_TYPE; // Removing the newly spawned enemy from the queue
	//	}
	//}

	ListItem<EntitySpawnpoint>* list;
	list = spawnQueue.start;

	for (int i = 0; i < spawnQueue.Count(); ++i)
	{
		if (list->data.type != EntityType::NO_TYPE)
		{
			SpawnEnemy(list->data);
			list->data.type = EntityType::NO_TYPE; // Removing the newly spawned enemy from the queue
		}
		list = list->next;
	}
}

void Entities::HandleEnemiesDespawn()
{
	// Iterate existing enemies
	//for (uint i = 0; i < MAX_ENEMIES; ++i)
	//{
	//	if (enemies[i] != nullptr)
	//	{
	//		// Delete the enemy when it has reached the end of the screen
	//		if (enemies[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
	//		{
	//			LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);

	//			enemies[i]->SetToDelete();
	//		}
	//	}
	//}

	ListItem<Entity*>* list;
	list = entities.start;
	for (int i = 0; i < entities.Count(); ++i)
	{
		if (list != NULL)
		{
			if (list->data->position.x * app->win->GetScale() < (app->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", list->data->position.x * app->win->GetScale());

				list->data->SetToDelete();
			}
		}
		list = list->next;
	}
}

void Entities::SpawnEnemy(const EntitySpawnpoint& info)
{
	// Find an empty slot in the enemies array
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] == nullptr)
		{
			switch (info.type)
			{
				case EntityType::ENEMY_FIREMINION:
				{
					enemies[i] = new TestEntityEnemy(info.x, info.y);
					enemies[i]->destroyedFx = enemyDestroyedFx;
					break;
				}
				case EntityType::ITEM_NUT:
				{
					enemies[i] = new Item_Nut(info.x, info.y);
					enemies[i]->texture = texture;
					enemies[i]->destroyedFx = itemPickedFx;;
					break;
				}
			}
			enemies[i]->texture = texture;
			break;
		}
	}*/
	Entity* newEntity = NULL;
	switch (info.type)
	{
	case EntityType::ENEMY_FIREMINION:
	{
		newEntity = new TestEntityEnemy(info.x, info.y);
		newEntity->destroyedFx = enemyDestroyedFx;
		newEntity->texture = texture;

		break;
	}
	case EntityType::ITEM_NUT:
	{
		newEntity = new Entity(info.x, info.y);
		newEntity->texture = texture;
		newEntity->destroyedFx = itemPickedFx;;
		break;
	}
	}
	entities.Add(newEntity);
}

bool Entities::OnCollision(Collider* c1, Collider* c2)
{
	//for(uint i = 0; i < MAX_ENEMIES; ++i)
	//{
	//	if(enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
	//	{
	//		enemies[i]->OnCollision(c2); //Notify the enemy of a collision
	//		break;
	//	}
	//}

	ListItem<Entity*>* entitiesList;
	entitiesList = entities.start;
	for (int i = 0; i < entities.Count(); ++i)
	{
		for (ListItem<Collider*>*collsList = app->collisions->colliders.start; collsList != NULL ; collsList = collsList->next)
		{
			if (entitiesList->data->GetCollider()->Intersects(collsList->data->rect))
			{
				entitiesList->data->OnCollision(collsList->data);
			}
			collsList = collsList->next;
		}
		entitiesList = entitiesList->next;
	}

	return true;
}