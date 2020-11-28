#include "ItemStar.h"

#include "App.h"
#include "ModuleCollisions.h"


ItemStar::ItemStar(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;

	anim.PushBack({ 41,0,32,30 });
	currentAnim = &anim;

	collider = app->collisions->AddCollider({ 0, 0, 32, 30 }, Collider::Type::ITEM, (Module*)app->entities, Collider::Items::STAR);
}

void ItemStar::Update()
{

	Entity::Update();
}