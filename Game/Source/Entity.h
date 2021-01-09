#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
#include "SString.h"
#include "Timer.h"

struct SDL_Texture;
struct Collider;
enum EntityType;

class Entity
{
public:

	enum DirectionState { STOP, LEFT, RIGHT, UP, DOWN, };
	// Constructor
	// Saves the spawn position for later movement calculations
	Entity(int x, int y);

	// Destructor
	virtual ~Entity();

	// Returns the enemy's collider
	const Collider* GetCollider() const;

	// Called from inhering enemies' Udpate
	// Updates animation and collider position
	virtual void Update(float dt);

	// Called from ModuleEnemies' Update
	virtual void Draw();

	// Collision response
	virtual void OnCollision(Collider* collider);

	// Sets flag for deletion and for the collider aswell
	virtual void SetToDelete();

	bool CheckRectEqual(SDL_Rect &a, SDL_Rect &b);

public:
	// The current position in the world
	fPoint position;

	// The enemy's itemsTexture
	SDL_Texture* texture = nullptr;
	SDL_Texture* debugTexture = nullptr;
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
	fPoint spawnPos;
	fPoint entitySpeed = { 0,0 };

	DirectionState direction = STOP;
	float counter = 0;
	bool inCollision = false;

	fPoint drawOffset = { 0,0 };
	fPoint collOffset = { 0,0 };
};

#endif // __ENEMY_H__