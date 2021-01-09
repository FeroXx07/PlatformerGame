#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
#include "SString.h"
#include "Timer.h"
#include "Input.h"

struct SDL_Texture;
struct Collider;

enum EntityType
{
	UNKNOWN,
	PLAYER,
	BULLET,
	ENEMY_WALKING,
	ENEMY_FLYING,
	ITEM_HEALTH,
	ITEM_STAR,
	HUD,
	MAP,
};

class Entity
{
public:
	Entity() {};
	Entity(EntityType type_) : type(type_) {}
	virtual ~Entity();

	EntityType type = EntityType::UNKNOWN;
	fPoint position;
	SDL_Texture* sprite = nullptr;
	Collider** entityCollider = nullptr; 

	virtual bool Update(float dt);
	virtual bool Draw();
	virtual bool HandleInput(float dt);

	virtual void OnCollision(Collider* collider);
	bool pendingToDelete = false;
	bool isDead = false;

	void SetToDelete();
};


#endif // __ENEMY_H_