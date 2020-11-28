#include "ItemHealth.h"

#include "App.h"
#include "ModuleCollisions.h"


ItemHealth::ItemHealth(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;

	anim.PushBack({ 97,123,22,22 });
	currentAnim = &anim;

	collider = app->collisions->AddCollider({0, 0, 22, 22 }, Collider::Type::ITEM, (Module*) app->entities, Collider::Items::HEALTH);
}

void ItemHealth::Update()
{
	
	// Fire Minion position update
	//if (spawnDelay >  60) {
	//	position.x += enemySpeed.x;
	//}
	//spawnDelay++;

	//// Fire Minion animations
	//if (App->hammer->hammerExist)
	//{
	//	if (enemySpeed.x < 0)
	//		currentAnim = &enemy_FireMinionLeftAnimHammer;
	//	else if (enemySpeed.x > 0) currentAnim = &enemy_FireMinionRightAnimHammer;
	//}
	//else
	//{
	//	if (enemySpeed.x < 0) 
	//		currentAnim = &anim;
	//	else if (enemySpeed.x > 0) currentAnim = &enemy_FireMinionRightAnim;
	//}
	//if (/*			!isGround			*//*			climbingUP || climbingDOWN			*/0) {
	//	collider->rect.w = 5;
	//	tempx = 4;
	//}
	//else {
	//	collider->rect.w = 15;
	//	tempx = 0;
	//}
	//collider->SetPos(position.x + tempx, position.y);
	Entity::Update();
}
