#include "Entity.h"

#include "App.h"
#include "ModuleCollisions.h"
#include "Audio.h"
#include "Render.h"
#include "ModuleFonts.h"
#include "Log.h"

//#include "ModuleParticles.h"
//include scene 
#include "ModuleEntities.h"


Entity::Entity(int x, int y) : position(x, y)
{
	spawnPos = position;
}

Entity::~Entity()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Update(float dt)
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (collider != nullptr)
		collider->SetPos(position.x + collOffset.x, position.y);
}

void Entity::Draw()
{
	if (currentAnim != nullptr)
	{
		app->render->DrawTexture(texture, position.x + drawOffset.x, position.y, &(currentAnim->GetCurrentFrame()));
	}

}

void Entity::OnCollision(Collider* collider)
{
	//Check collision type
	if (this->collider->type == Collider::ITEM && collider->type == Collider::Type::PLAYER)
	{
		switch (this->collider->item)
		{
		case Collider::Items::HEALTH:
			this->SetToDelete();
			isDead = true;
			break;
		case Collider::Items::STAR:
			this->SetToDelete();
			isDead = true;
			break;
		default:
			this->SetToDelete();
			break;
		}
		this->SetToDelete();
			
	}

	if (this->collider->type == Collider::Type::ENEMY_HURTBOX && collider->type == Collider::Type::BULLET )
	{
		if (collider->pendingToDelete == false)
		{
			LOG("DAAAAAAAAAAAAAAAAMAAAAAAAAAAAAAAAGE");
			if (this->entityType == ENEMY_WALKING || this->entityType == ENEMY_FLYING)
			{
				if (health > 300) health = 300;
				this->health += -100;
			}
		}
	}
}

void Entity::SetToDelete()
{
	pendingToDelete = true;
	if (collider != nullptr)
		collider->pendingToDelete = true;
}

bool Entity::CheckRectEqual(SDL_Rect& a, SDL_Rect& b)
{
	if (a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h)
	{
		return true;
	}
	return false;
}