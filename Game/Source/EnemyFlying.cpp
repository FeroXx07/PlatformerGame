#include "EnemyFlying.h"

#include "App.h"
#include "ModuleCollisions.h"
#include "Pathfinding.h"
#include "ModulePlayer.h"
#include "Map.h"
#include "Render.h"
#include "Input.h"

#include "Log.h"

#define SPEED_X 60.0f;
#define SPEED_Y 30.0f;

EnemyFlying::EnemyFlying(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;

	anim.PushBack({0,83,64,38});
	anim.PushBack({0,44,64,38});
	anim.PushBack({0,0,64,43});
	anim.loop = true;

	deathAnim.PushBack({ 0,147,60,25 });
	deathAnim.loop = false;

	anim.speed = 0.10f;
	currentAnim = &anim;
	health = 300;

	collider = app->collisions->AddCollider({0, 0, 64, 38 }, Collider::Type::ENEMY_HURTBOX, (Module*) app->entities, Collider::Items::ITEM_NONE);
}

Entity::DirectionState EnemyFlying::Gps(iPoint &p)
{
	if (this->position.y - 12 < p.y)
		vertical = DOWN;
	else
		vertical = UP;

	if (this->position.x > p.x)
		return DirectionState::LEFT;
	else
		return DirectionState::RIGHT;
}

void EnemyFlying::SetVelDirection(DirectionState dir)
{
	if (dir == RIGHT)
	{
		this->entitySpeed.x = SPEED_X;
		if (vertical == UP)
		{
			this->entitySpeed.y = -SPEED_Y;
		}
		else if (vertical == DOWN)
		{
			this->entitySpeed.y = SPEED_Y;
		}
	}
	else if (dir == LEFT)
	{
		this->entitySpeed.x = -SPEED_X;
		if (vertical == UP)
		{
			this->entitySpeed.y = -SPEED_Y;
		}
		else if (vertical == DOWN)
		{
			this->entitySpeed.y = SPEED_Y;
		}
	}
}
void EnemyFlying::Update(float dt)
{
	Entity::Update(dt);

	if (currentAnim == &anim)
	{
		SDL_Rect b = SDL_Rect({ 65, 0, 49, 38 });
		SDL_Rect a = SDL_Rect(currentAnim->GetCurrentFrame());
		if (CheckRectEqual(a, b))
		{
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
		this->collider->pendingToDelete = true;
	}

	// Manually get pathfinding
	if ((app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN || hasPath == false) && isDead == false)
	{
		// Clear any old path from pathfinding class
		app->pathfinding->lastPath.Clear();

		// Conversion
		iPoint dst = { (int)app->player->playerPos.x,(int)app->player->playerPos.y + 32};
		dst = app->map->WorldToMap(dst.x, dst.y);

		// Conversion
		iPoint origin = app->map->WorldToMap(this->position.x, this->position.y);
		app->pathfinding->CreatePathFlying(origin, dst);

		// Clear current path
		path.Clear();

		// Add eachpath points to our own path dyn array
		for (int i = 0; i < app->pathfinding->GetLastPath()->Count(); ++i)
		{
			path.PushBack(*app->pathfinding->lastPath.At(i));
		}
		path.Flip();
		hasPath = true;
	}

	if (path.Count() > 0 )
	{
		iPoint nextPos; 
		if (direction == STOP)
		{
			// Get directions if current direction is stop
			if (path.Pop(nextPos))
			{
				direction = this->Gps(app->map->MapToWorld(nextPos.x,nextPos.y));
			}
		}
		
		// Move with those directions for some time
		if (direction != STOP)
		{
			SetVelDirection(direction);
			counter += 1.0f * dt;
		}
		
		// After time ended reset directions to stop
		
		if (counter >= 1.0f) // 1.0f too long
		{
			direction = STOP;
			counter = 0.0f;
		}
	}
	else
	{
		direction = STOP;
		hasPath = false;
	}

	if (direction == STOP)
	{
		entitySpeed = { 0,0 };
	}

	if (app->collisions->debug)
	{
		for (uint i = 0; i < path.Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path.At(i)->x, path.At(i)->y);
			app->render->DrawTexture(debugTexture, pos.x, pos.y);
		}
	}

	if (isDead == false)
	{
		position.x = position.x + entitySpeed.x * dt;
		position.y = position.y + entitySpeed.y * dt;
	}

	collider->SetPos(position.x, position.y);
}
