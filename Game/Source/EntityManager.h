#ifndef __EntityManager_H__
#define __EntityManager_H__

#include "SDL/include/SDL_rect.h"

#include "Module.h"
#include "Textures.h"
#include "Entity.h"
#include "List.h"

struct SDL_Rect;

class EntityManager : public Module
{
public:
	EntityManager(bool enabled);
	~EntityManager();

	List<Entity*>entities;
	
	Entity* CreateEntity(EntityType type, int x, int y, bool isDead = false);
	void DestroyEntity(Entity* entity);

	bool Start();

	bool Update(float dt);
	
	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	void CheckCollisions();
	void DeletePendingEntities();
public:
	void UpdateAll(float dt, bool doLogic);

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	SDL_Texture* enemySpritsheet = nullptr;
	SDL_Texture* itemSpritsheet = nullptr;

	bool OnCollision(Collider* c1, Collider* c2);
};

#endif