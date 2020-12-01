#ifndef __MODULE_ENTITIES_H__
#define __MODULE_ENTITIES_H__

#include "Module.h"
#include "List.h"



enum class EntityType
{
	NO_TYPE,
	ITEM_HEALTH,
	ITEM_STAR,
};

struct EntitySpawnpoint
{
	EntityType type = EntityType::NO_TYPE;
	int x, y;
};

class Entity;
struct SDL_Texture;

class Entities : public Module
{
public:
	// Constructor
	Entities(bool startEnabled);

	// Destructor
	~Entities();

	// Called when the module is activated
	// Loads the necessary textures for the enemies
	bool Start() override;

	// Called at the beginning of the application loop
	// Removes all enemies pending to delete
	bool PreUpdate() override;

	// Called at the middle of the application loop
	// Handles all enemies logic and spawning/despawning
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Iterates all the enemies and draws them
	bool PostUpdate() override;

	// Called on application exit
	// Destroys all active enemies left in the array
	bool CleanUp() override;

	// Called when an enemi collider hits another collider
	// The enemy is destroyed and an explosion particle is fired
	bool OnCollision(Collider* c1, Collider* c2);

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	// Add an enemy into the queue to be spawned later
	bool AddEntity(EntityType type, int x, int y);

	// Iterates the queue and checks for camera position
	void HandleEnemiesSpawn();

	// Destroys any enemies that have moved outside the camera limits
	void HandleEnemiesDespawn();

private:
	// Spawns a new enemy using the data from the queue
	void SpawnEnemy(const EntitySpawnpoint& info);

private:
	// A queue with all spawn points information
	/*EntitySpawnpoint spawnQueue[MAX_ENEMIES];*/
	List<EntitySpawnpoint>spawnQueue;

	// All spawned enemies in the scene
	/*Entity* enemies[MAX_ENEMIES] = { nullptr };*/
	List<Entity*>entities;

	// The enemies sprite sheet
	SDL_Texture* texture = nullptr;

	// The audio fx for destroying an enemy/item
	int enemyDestroyedFx = 0;

	// The audio fx for picking an item
	int itemPickedFx = 0;
};

#endif // __MODULE_ENTITIES_H__