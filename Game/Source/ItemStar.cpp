#include "EnemiesClasses.h"

#include "App.h"
#include "ModuleCollisions.h"


ItemStar::ItemStar() : Enemy(EntityType::ITEM_STAR)
{
	spawnDelay = 0;

	anim.PushBack({ 41,0,32,30 });
	currentAnim = &anim;

	collider = app->collisions->AddCollider({ 0, 0, 32, 30 }, Collider::Type::ITEM, (Module*)app->entityman, Collider::Items::STAR);
	this->entityCollider = &collider;
}

bool ItemStar::Update(float dt)
{
	this->EnemyUpdate(dt);
	return true;
}

bool ItemStar::Draw()
{
	this->EnemyDraw();
	return false;
}
