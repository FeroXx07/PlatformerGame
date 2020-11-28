#include "Entity.h"

#include "App.h"
#include "ModuleCollisions.h"
#include "Audio.h"
#include "Render.h"
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

void Entity::Update()
{
	if (currentAnim != nullptr)
		currentAnim->Update();

	if (collider != nullptr)
		collider->SetPos(position.x + tempx, position.y);
}

void Entity::Draw()
{
	if (currentAnim != nullptr)
		app->render->DrawTexture(texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));
}

void Entity::OnCollision(Collider* collider)
{
	//Check collision type
	if (this->collider->type == Collider::ITEM && collider->type==Collider::Type::PLAYER)
	{
		switch (this->collider->item)
		{
		case Collider::Items::HEALTH:
			this->SetToDelete();
			break;
		case Collider::Items::STAR:
			this->SetToDelete();
			break;
		default:
			break;
		}
	}
}

void Entity::SetToDelete()
{
	pendingToDelete = true;
	if (collider != nullptr)
		collider->pendingToDelete = true;
}