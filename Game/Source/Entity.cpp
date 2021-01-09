#include "Entity.h"

#include "App.h"
#include "ModuleCollisions.h"
#include "Audio.h"
#include "Render.h"
#include "Textures.h"
#include "Log.h"

Entity::~Entity()
{
	entityCollider = nullptr;
}

bool Entity::Update(float dt)
{
	return true;
}

bool Entity::Draw()
{
	return true;
}

bool Entity::HandleInput(float dt)
{
	return true;
}

void Entity::OnCollision(Collider* collider)
{
}

void Entity::SetToDelete()
{
	pendingToDelete = true;
	if (entityCollider != nullptr)
	{
		Collider* rect = *entityCollider;
		rect->pendingToDelete = true;
	}
}
