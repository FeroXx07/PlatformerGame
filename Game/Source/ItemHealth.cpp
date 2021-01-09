#include "EnemiesClasses.h"

#include "App.h"
#include "ModuleCollisions.h"


ItemHealth::ItemHealth() : Enemy(EntityType::ITEM_HEALTH)
{
	spawnDelay = 0;

	anim.PushBack({ 97,123,22,22 });
	currentAnim = &anim;
	
	collider = app->collisions->AddCollider({0, 0, 22, 22 }, Collider::Type::ITEM, (Module*) app->entityman, Collider::Items::HEALTH);
	this->entityCollider = &collider;
}

bool ItemHealth::Update(float dt)
{
	this->EnemyUpdate(dt);
	return true;
}

bool ItemHealth::Draw()
{
	this->EnemyDraw();
	return false;
}