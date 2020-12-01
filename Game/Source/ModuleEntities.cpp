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

#include "ItemHealth.h"
#include "ItemStar.h"
//#include "Item_Iron.h"
//#include "Item_Umbrella.h"
//#include "Item_Bag.h"
//#include "Item_Hammer.h"
//#include "Enemy_Minecart.h"


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
	/*texture = app->tex->Load("Assets/Background2.png");
	
	enemyDestroyedFx = app->audio->LoadFx("Assets/Music/SFX_Kill.wav");
	
	itemPickedFx = app->audio->LoadFx("Assets/Music/SFX_Bonus.wav");*/
	texture = app->tex->Load("Assets/Common/spritesheet_items.png");
	itemPickedFx = app->audio->LoadFx("Assets/Audio/Fx/item_taken.wav");


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
	//app->audio->UnloadFx(itemPickedFx);
	
	app->tex->UnLoad(texture);


	LOG("Freeing all enemies");

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
	Entity* newEntity = NULL;
	switch (info.type)
	{
	case EntityType::ITEM_HEALTH:
	{
		newEntity = new ItemHealth(info.x, info.y);
		newEntity->destroyedFx = itemPickedFx;
		newEntity->texture = texture;

		break;
	}
	case EntityType::ITEM_STAR:
	{
		newEntity = new ItemStar(info.x, info.y);
		newEntity->texture = texture;
		newEntity->destroyedFx = itemPickedFx;
		break;
	}
	}
	entities.Add(newEntity);
}

bool Entities::OnCollision(Collider* c1, Collider* c2) // This is called through listener from the OnCollision(c2,c1) of player.cpp c2->entity rect c1->player rect
{
	ListItem<Entity*>* entitiesList;
	entitiesList = entities.start;
	for (int i = 0; i < entities.Count(); ++i)
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
	// First change total num of ent
	pugi::xml_node entityCount;
	entityCount = data;
	entityCount.child("entitiesCount").attribute("Num").set_value(this->entities.Count());
	
	// Then erase all ent in the xml
	pugi::xml_node listEnt;
	listEnt = data.child("entitiesList");
	for (int i=0; i< MAX_ENTITIES;++i)
	{
		bool remove = listEnt.remove_child("entities");
		if (remove == false)
			break;
	}

	// Finally add all ent in the xml
	ListItem<Entity*>* list;
	list = entities.start;
	for (int i = 0; i < this->entities.Count(); ++i)
	{
		list->data = entities.At(i)->data;

		pugi::xml_node newEnt = data.child("entitiesList");
		newEnt = newEnt.append_child("entities");
		newEnt.append_attribute("Num").set_value(i);
		newEnt.append_attribute("Item").set_value(list->data->GetCollider()->item);
		newEnt.append_attribute("Type").set_value(list->data->GetCollider()->type);
		newEnt.append_attribute("x").set_value(list->data->GetCollider()->rect.x);
		newEnt.append_attribute("y").set_value(list->data->GetCollider()->rect.y);

	}
	return ret;
}

bool Entities::LoadState(pugi::xml_node& data)  // Node is pointing to "entity"
{
	bool ret = true;

	return ret;
}