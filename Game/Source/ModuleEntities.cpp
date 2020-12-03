#include "ModuleEntities.h"

#include "App.h"

#include "Render.h"
#include "Window.h"
#include "Textures.h"
#include "Audio.h"
#include "Log.h"
#include "List.h"
#include "ModuleCollisions.h"
#include "ModuleFonts.h"

#include "Entity.h"

#include "ItemHealth.h"
#include "ItemStar.h"
#include "EnemyWalking.h"


#define SPAWN_MARGIN 50
#define MAX_ENTITIES 20

Entities::Entities(bool startEnabled) : Module(startEnabled)
{
	name.Create("entity");
	
}

Entities::~Entities()
{

}

bool Entities::Start()
{
	/*itemsTexture = app->tex->Load("Assets/Background2.png");
	
	enemyDestroyedFx = app->audio->LoadFx("Assets/Music/SFX_Kill.wav");
	
	itemPickedFx = app->audio->LoadFx("Assets/Music/SFX_Bonus.wav");*/
	itemsTexture = app->tex->Load("Assets/Common/spritesheet_items.png");
	itemPickedFx = app->audio->LoadFx("Assets/Audio/Fx/item_taken.wav");

	enemiesTexture = app->tex->Load("Assets/Common/spritesheet_enemies.png");
	char lookupTableNumbers[] = { "0123456789" };
	//whiteFont = app->fonts->Load("Assets/Fonts/fonts_white.png", lookupTableNumbers, 1);

	char lookupTableTextAndLives[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ.-" };
	//yellowFont = app->fonts->Load("Assets/Fonts/fonts_yellow.png", lookupTableTextAndLives, 1);

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
			delete list->data;
		}
		list = list->next;
	}

	return ret;
}

bool Entities::Update(float dt)
{
	bool ret = true;
	HandleEnemiesSpawn();

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
	//app->audio->UnloadFx(itemPickedFx);
	
	app->tex->UnLoad(itemsTexture);


	LOG("Freeing all enemies");

	ListItem<Entity*>* list;
	list = entities.start;
	for (int i = 0; i < entities.Count(); ++i)
	{
		if (list != NULL)
		{
			entities.Del(list);
			delete list->data;
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
	Entity* newEntity = NULL;
	switch (info.type)
	{
		case EntityType::ITEM_HEALTH:
		{
			newEntity = new ItemHealth(info.x, info.y);
			newEntity->destroyedFx = itemPickedFx;
			newEntity->texture = itemsTexture;
			newEntity->name = "Health";
			newEntity->entityType = EntityType::ITEM_HEALTH;
			break;
		}
		case EntityType::ITEM_STAR:
		{
			newEntity = new ItemStar(info.x, info.y);
			newEntity->texture = itemsTexture;
			newEntity->name = "Star";
			newEntity->destroyedFx = itemPickedFx;
			newEntity->entityType = EntityType::ITEM_STAR;
			break;
		}
		case EntityType::ENEMY_WALKING:
		{
			newEntity = new EnemyWalking(info.x, info.y);
			newEntity->texture = enemiesTexture;
			newEntity->name = "EnemyWalking";
			newEntity->destroyedFx = itemPickedFx;
			newEntity->entityType = EntityType::ENEMY_WALKING;
			break;
		}
	}
	entities.Add(newEntity);
}

bool Entities::OnCollision(Collider* c1, Collider* c2) // This is called through listener from the OnCollision(c2,c1) of player.cpp c2->entity rect c1->player rect
{
	ListItem<Entity*>* entitiesList;
	entitiesList = entities.start;
	int count = entities.Count();
	for (int i = 0; i <count; ++i)
	{
		entitiesList = entities.At(i);
		if (entitiesList->data->GetCollider() == c1)
			entitiesList->data->OnCollision(c2);
	}
	
	/*for (int i = 0; i < ent.Count(); ++i)
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
	}*/
	
	return true;
}

bool Entities::SaveState(pugi::xml_node& data) const // Node is pointing to "entity"
{
	bool ret = true;
	// First change total num of entities in the xml
	pugi::xml_node entityCount;
	entityCount = data;
	entityCount.child("entitiesCount").attribute("Num").set_value(this->entities.Count());
	
	// Then erase entities ent in the xml
	pugi::xml_node listEnt;
	listEnt = data.child("entitiesList");
	for (int i=0; i< MAX_ENTITIES;++i)
	{
		bool remove = listEnt.remove_child("entities");
		if (remove == false)
			break;
	}

	// Finally add all entities in the xml
	ListItem<Entity*>* list;
	list = entities.start;
	int total = entities.Count();
	for (int i = 0; i < total; ++i)
	{
		list->data = entities.At(i)->data;

		pugi::xml_node newEnt = data.child("entitiesList");
		newEnt = newEnt.append_child("entities");
		newEnt.append_attribute("Num").set_value(i);
		newEnt.append_attribute("Type").set_value(list->data->entityType);
		newEnt.append_attribute("x").set_value(list->data->GetCollider()->rect.x);
		newEnt.append_attribute("y").set_value(list->data->GetCollider()->rect.y);

	}
	return ret;
}

bool Entities::LoadState(pugi::xml_node& data)  // Node is pointing to "entity"
{
	bool ret = true;
	int total = entities.Count(); // Don't put entities.Count() inside 
	// Delete all current entities from the world
	ListItem<Entity*>* toDelete;
	toDelete = entities.start;
	for (int i = 0; i < total; ++i)
	{
		toDelete->data->SetToDelete();
		entities.Del(toDelete);
		toDelete = toDelete->next;
	}
	entities.Clear();
	// Then add all entities from the xml
	pugi::xml_node count;
	count = data.child("entitiesCount");
	int newTotal = count.attribute("Num").as_int();

	ListItem<Entity*>* list;
	list = entities.start;
	
	pugi::xml_node newEnt = data.child("entitiesList").first_child();
	for (int i = 0; i < newTotal; ++i)
	{
		EntityType type = static_cast<EntityType>(newEnt.attribute("Type").as_int());
		iPoint position{ 0,0 };
		position.x = newEnt.attribute("x").as_int();
		position.y = newEnt.attribute("y").as_int();
		AddEntity(type, position.x, position.y);
		
		newEnt = newEnt.next_sibling();
	}
	return ret;
}