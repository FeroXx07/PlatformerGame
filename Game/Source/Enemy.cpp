#include "Enemy.h"

#include "App.h"
#include "ModuleCollisions.h"
#include "Audio.h"
#include "Render.h"
#include "Log.h"
#include "Textures.h"
#include "EntityManager.h"

Enemy::Enemy()
{
}
Enemy::Enemy(EntityType t) : Entity(t)
{
	spawnPos = position;
	debugTexture = app->tex->Load("Assets/maps/x_img.png");
}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
	this->entityCollider = nullptr;
	app->tex->UnLoad(debugTexture);
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::EnemyUpdate(float dt)
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (collider != nullptr)
		collider->SetPos(position.x + collOffset.x, position.y);
}

void Enemy::EnemyDraw()
{
	if (currentAnim != nullptr && sprite != NULL)
	{
		app->render->DrawTexture(sprite, position.x + drawOffset.x, position.y, &(currentAnim->GetCurrentFrame()));
	}

}

void Enemy::OnCollision(Collider* collider)
{
	//Check collision type
	if (this->collider->type == Collider::ITEM && collider->type == Collider::Type::PLAYER)
	{
		switch (this->collider->item)
		{
		case Collider::Items::HEALTH:
			app->entityman->DestroyEntity(this);
			isDead = true;
			break;
		case Collider::Items::STAR:
			app->entityman->DestroyEntity(this);
			isDead = true;
			break;
		default:
			app->entityman->DestroyEntity(this);
			break;
		}
		app->entityman->DestroyEntity(this);
	}

	if (this->collider->type == Collider::Type::ENEMY_HURTBOX && collider->type == Collider::Type::BULLET)
	{
		if (collider->pendingToDelete == false)
		{
			LOG("DAAAAAAAAAAAAAAAAMAAAAAAAAAAAAAAAGE");
			if (this->type == EntityType::ENEMY_WALKING || this->type == EntityType::ENEMY_FLYING)
			{
				if (health > 300) health = 300;
				this->health += -100;
			}
		}
	}
}

void Enemy::SetToDelete()
{
	pendingToDelete = true;

	if (collider != nullptr)
	{
		collider->pendingToDelete = true;
		this->entityCollider = nullptr;
	}
}

bool Enemy::CheckRectEqual(SDL_Rect& a, SDL_Rect& b)
{
	if (a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h)
	{
		return true;
	}
	return false;
}