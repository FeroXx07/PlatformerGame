#include "EnemyWalking.h"

#include "App.h"
#include "ModuleCollisions.h"
#include "Log.h"
EnemyWalking::EnemyWalking(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;

	anim.PushBack({ 158,106,32,44});
	anim.PushBack({ 158,151,32,42 });
	anim.PushBack({ 65,0,49,38 });
	anim.PushBack({ 158,151,32,42 });
	anim.loop = true;

	deathAnim.PushBack({ 117,154,35,41});
	deathAnim.loop = false;

	anim.speed = 0.10f;
	currentAnim = &anim;
	health = 300;

	collider = app->collisions->AddCollider({0, 0, 32, 44 }, Collider::Type::ENEMY_HURTBOX, (Module*) app->entities, Collider::Items::ITEM_NONE);
}

void EnemyWalking::Update()
{
	Entity::Update();

	if (currentAnim == &anim)
	{
		SDL_Rect b = SDL_Rect({ 65, 0, 49, 38 });
		SDL_Rect a = SDL_Rect(currentAnim->GetCurrentFrame());
		if (CheckRectEqual(a, b))
		{
			LOG("OFFFFFSET");
			drawOffset.x = -9;
		}
		else
			drawOffset.x = 0;
	}

	if (health <= 0)
	{
		isDead = true;
		this->collider->pendingToDelete = true;
		//this->pendingToDelete = true;;
	}

	if (isDead)
	{
		currentAnim = &deathAnim;
	}

	collider->SetPos(position.x, position.y);
}
