#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
#include "SString.h"

struct SDL_Texture;
struct Collider;
enum EntityType;

class Entity
{
public:
	// Constructor
	// Saves the spawn position for later movement calculations
	Entity(int x, int y);

	// Destructor
	virtual ~Entity();

	// Returns the enemy's collider
	const Collider* GetCollider() const;

	// Called from inhering enemies' Udpate
	// Updates animation and collider position
	virtual void Update();

	// Called from ModuleEnemies' Update
	virtual void Draw();

	// Collision response
	virtual void OnCollision(Collider* collider);

	// Sets flag for deletion and for the collider aswell
	virtual void SetToDelete();

	bool CheckRectEqual(SDL_Rect &a, SDL_Rect &b);
public:
	// The current position in the world
	iPoint position;

	// The enemy's itemsTexture
	SDL_Texture* texture = nullptr;
	
	// Sound fx when destroyed
	int destroyedFx = 0;

	// A flag for the enemy removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

	EntityType entityType;

	SString name = SString("NONE");
	int health = 0;

	// The enemy's collider
	Collider* collider = nullptr;

	bool isDead = false;
protected:
	// A ptr to the current animation
	Animation* currentAnim = nullptr;

	// Original spawn position. Stored for movement calculations
	iPoint spawnPos;
	iPoint entitySpeed = { 0,0 };

	enum DirectionState { LEFT,RIGHT,UP,DOWN, };

	bool inCollision = false;

	iPoint drawOffset = { 0,0 };
	iPoint collOffset = { 0,0 };
};

#endif // __ENEMY_H__