#include "EnemyWalking.h"

#include "App.h"
#include "ModuleCollisions.h"


EnemyWalking::EnemyWalking(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;

	anim.PushBack({158,106,32,44});
	currentAnim = &anim;
	
	collider = app->collisions->AddCollider({0, 0, 32, 44 }, Collider::Type::ENEMY_HURTBOX, (Module*) app->entities, Collider::Items::ITEM_NONE);
}

void EnemyWalking::Update()
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
	//	offsetX = 4;
	//}
	//else {
	//	collider->rect.w = 15;
	//	offsetX = 0;
	//}
	collider->SetPos(position.x + offsetX, position.y);
	Entity::Update();
}
