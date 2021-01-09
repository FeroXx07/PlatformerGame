#include "EntityManager.h"
#include "App.h"
#include "Input.h"
#include "Window.h"
#include "Render.h"
#include "Audio.h"
#include "Log.h"
#include "LevelScene.h"
#include "Player.h"
#include "EnemiesClasses.h"
#include "Hud.h"
#include "Bullet.h"
#include "SceneManager.h"
#include "Collider.h"

#define MAX_ENTITIES 50

EntityManager::EntityManager(bool b) : Module(b)
{
	name = "entity";
}

EntityManager::~EntityManager() 
{
	CleanUp();
}

Entity* EntityManager::CreateEntity(EntityType type, int x, int y, bool isDead)
{
	Entity* ret = nullptr;

	switch (type)
	{
	//case EntityType::NPC: ret = new NPC(); break;
	case EntityType::PLAYER: ret = new Player(); break;
	case EntityType::ENEMY_WALKING: 
	{
		ret = new EnemyWalking(); 
		ret->sprite = enemySpritsheet;
		break;
	}
	case EntityType::ENEMY_FLYING:
	{
		ret = new EnemyFlying();
		ret->sprite = enemySpritsheet;
		break;
	}
	case EntityType::BULLET:
	{
		ret = new Bullet();
		ret->sprite = itemSpritsheet;
		break;
	}
	case EntityType::ITEM_STAR:
	{
		ret = new ItemStar();
		ret->sprite = itemSpritsheet;
		break;
	}
	case EntityType::ITEM_HEALTH:
	{
		ret = new ItemStar();
		ret->sprite = itemSpritsheet;
		break;
	}
	case EntityType::HUD: ret = new Hud(); break;
	}

	if (ret != nullptr)
	{
		ret->isDead = isDead;
		ret->position.x = x;
		ret->position.y = y;
		entities.Add(ret);
	}
	return ret;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	entity->SetToDelete();
}

bool EntityManager::Start()
{
	enemySpritsheet = app->tex->Load("Assets/Characters/spritesheet_enemies.png");
	itemSpritsheet = app->tex->Load("Assets/Common/spritesheet_items.png");

	return true;
}

void EntityManager::UpdateAll(float dt, bool doLogic)
{
	ListItem<Entity*>* list;
	for (list = entities.start; list != NULL; list = list->next)
	{
		if (doLogic)
		{
			list->data->HandleInput(dt);
			list->data->Update(dt);
			list->data->Draw();
		}
	}
}

bool EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	return false;
}

bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	DeletePendingEntities();
	CheckCollisions();
	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}
	LOG("TOTAL ENTITIES: %d", entities.Count());
	return true;
}

bool EntityManager::CleanUp()
{
	/*ListItem<Entity*>* list;
	for (list = entities.start; list != NULL; list = list->next)
	{
		entities.Del(list);
		RELEASE(list->data);
	}*/

	ListItem<Entity*>* toDelete;
	toDelete = entities.start;
	int total = entities.Count();
	for (int i = 0; i < total; ++i)
	{
	/*	toDelete->data->SetToDelete();
		toDelete = toDelete->next;*/
		toDelete = entities.At(i);
		toDelete->data->~Entity();
		RELEASE(toDelete->data);
		delete toDelete->data;
	}

	entities.Clear();

	app->tex->UnLoad(enemySpritsheet);
	app->tex->UnLoad(itemSpritsheet);

	return false;
}

bool EntityManager::LoadState(pugi::xml_node& data)
{
	pugi::xml_node node = data;
	ListItem<Entity*>* list;
	for (list = entities.start; list != NULL; list = list->next)
	{
		if (list->data->type == EntityType::PLAYER)
		{
			Player* player = (Player*)list;
			player = app->sceneManager->playerScene;
			player->LoadState(node);
			break;
		}
	}

	int total = entities.Count(); // Don't put entities.Count() inside 

	// Delete all previousScene entities from the world
	ListItem<Entity*>* toDelete;
	toDelete = entities.start;
	for (int i = 0; i < total; ++i)
	{
		if (toDelete->data->type == EntityType::PLAYER || toDelete->data->type == EntityType::HUD)
		{
			toDelete = toDelete->next;
			continue;
		}
		toDelete->data->SetToDelete();
		toDelete = toDelete->next;
	}

	// Then add all entities from the xml
	pugi::xml_node count;
	count = data.child("entitiesCount");
	int newTotal = count.attribute("Num").as_int();

	pugi::xml_node newEnt = data.child("entitiesList").first_child();
	for (int i = 0; i < newTotal; ++i)
	{
		EntityType type = static_cast<EntityType>(newEnt.attribute("Type").as_int());

		if (type != EntityType::PLAYER || type != EntityType::HUD)
		{
			iPoint position{ 0,0 };
			position.x = newEnt.attribute("x").as_int();
			position.y = newEnt.attribute("y").as_int();
			bool b = newEnt.attribute("dead").as_bool();
			CreateEntity(type, position.x, position.y, b);
		}
		newEnt = newEnt.next_sibling();
	}

	return true;
}

bool EntityManager::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node node = data;
	ListItem<Entity*>* list;
	for (list = entities.start; list != NULL; list = list->next)
	{
		if (list->data->type == EntityType::PLAYER)
		{
			Player* player = (Player*)list;
			player = app->sceneManager->playerScene;
			player->SaveState(node);
			break;
		}
	}

	pugi::xml_node entityCount;
	entityCount = data;
	entityCount.child("entitiesCount").attribute("Num").set_value(this->entities.Count());

	// Then erase entities ent in the xml
	pugi::xml_node listEnt;
	listEnt = data.child("entitiesList");
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		bool remove = listEnt.remove_child("entities");
		if (remove == false)
			break;
	}

	
	int total = entities.Count();
	int i = 0;
	ListItem<Entity*>* listTwo;
	for (listTwo = entities.start; listTwo != NULL; listTwo = listTwo->next)
	{
		if (listTwo->data->type == EntityType::PLAYER || listTwo->data->type == EntityType::BULLET || listTwo->data->type == EntityType::HUD)
		{
			++i;
			continue;
		}

		pugi::xml_node newEnt = data.child("entitiesList");
		newEnt = newEnt.append_child("entities");
		newEnt.append_attribute("Num").set_value(i);
		newEnt.append_attribute("Type").set_value(listTwo->data->type);

		if (listTwo->data->entityCollider != nullptr)
		{
			Collider* rect = *listTwo->data->entityCollider;
			newEnt.append_attribute("x").set_value(rect->rect.x);
			newEnt.append_attribute("y").set_value(rect->rect.y);
		}
		newEnt.append_attribute("dead").set_value(listTwo->data->isDead);
		++i;
	}

	return true;
}

void EntityManager::CheckCollisions()
{
	ListItem<Entity*>* listOne;
	for (listOne = entities.start; listOne != NULL; listOne = listOne->next)
	{
		ListItem<Entity*>* listTwo;
		for (listTwo = entities.start; listTwo != NULL; listTwo = listTwo->next)
		{
			//if ((listOne->data->type == EntityType::PLAYER) && (listTwo->data->type == EntityType::PLAYER))
			//	continue;

			if ((listOne->data == listTwo->data))
				continue;

			if ((listOne->data->entityCollider != nullptr) && (listTwo->data->entityCollider != nullptr))
			{
				Collider* rect1 = *listOne->data->entityCollider;
				Collider* rect2 = *listTwo->data->entityCollider;

				if (rect1->Intersects(rect2->rect)) // Collision
				{
					listOne->data->OnCollision(rect2);
					listTwo->data->OnCollision(rect1);
				}
			}
		}
	}
}

void EntityManager::DeletePendingEntities()
{
	ListItem<Entity*>* list;
	for (list = entities.start; list != NULL; list = list->next)
	{
		if (list->data->pendingToDelete == true)
		{
			entities.Del(list);
			delete list->data;
		}
	}
}

