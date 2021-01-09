#include "Bullet.h"
#include "App.h"
#include "ModuleCollisions.h"
#include "Pathfinding.h"
#include "Map.h"
#include "Render.h"
#include "Input.h"
#include "Player.h"
#include "LevelScene.h"
#include "Textures.h"
#include "Audio.h"

#include "Log.h"

Bullet::Bullet() : Entity(EntityType::BULLET)
{
	bulletAnim.PushBack({ 74,77,24,22 });
	bulletAnim.loop = false;
	bulletAnim.speed = 0.1f;
	lifetime = 60;
	collider = app->collisions->AddCollider({ 74,77,24,22 }, Collider::BULLET, (Module*)app->entityman);
	this->entityCollider = &collider;
}

Bullet::~Bullet()
{
}

bool Bullet::Update(float dt)
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the delay has been reached
	if (!isAlive && frameCount >= 0)
		isAlive = true;

	if (hasDied)
	{
		ret = false;
		collider->pendingToDelete = true;
		isAlive = false;
		this->pendingToDelete = true;
	}

	if (isAlive)
	{
		bulletAnim.Update();

		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifetime > 0)
		{
			if (frameCount >= lifetime)
			{
				ret = false;
				collider->pendingToDelete = true;
				if (entityCollider != nullptr)
					entityCollider = nullptr;
				this->pendingToDelete = true;
			}
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (bulletAnim.HasFinished())
		{
			ret = false;
			collider->pendingToDelete = true;
			if (entityCollider != nullptr)
				entityCollider = nullptr;
			this->pendingToDelete = true;
		}

		// Update the position in the screen
		position.x += speed.x * dt;
		position.y += speed.y * dt;

		if (collider != nullptr)
			collider->SetPos(position.x, position.y);
	}
	
	return ret;
}

bool Bullet::Draw()
{
	app->render->DrawTexture(sprite, position.x,position.y, &(bulletAnim.GetCurrentFrame()));
	return true;
}

bool Bullet::HandleInput(float dt)
{
	return true;
}

void Bullet::OnCollision(Collider* collider)
{
	if (collider->type != Collider::Type::PLAYER && collider->type == Collider::Type::ENEMY_HURTBOX)
	{
		// If you don't want to destroy something:
		switch (collider->type)
		{
		case Collider::Type::ENEMY_HURTBOX:
			LOG("BULLLLLLLLLLLLLLLLLLLLLLLET HIIIIIIIIIIIIIIIIIIIIIIT");
			app->audio->PlayFx(destroyed);
			this->hasDied = true;
			break;

		default:
			break;
		}
	}
}
