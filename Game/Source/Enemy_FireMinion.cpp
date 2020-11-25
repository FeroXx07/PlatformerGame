#include "Enemy_FireMinion.h"

#include "App.h"
#include "ModuleCollisions.h"


Enemy_FireMinion::Enemy_FireMinion(int x, int y) : Entity(x, y)
{
	spawnDelay = 0;

	enemy_FireMinionLeftAnim.PushBack({ 0,0,42,40 });
	currentAnim = &enemy_FireMinionLeftAnim;

	collider = app->collisions->AddCollider({0, 0, 42, 40}, Collider::Type::ENEMY, (Module*) app->entities);
}

void Enemy_FireMinion::Update()
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
	//		currentAnim = &enemy_FireMinionLeftAnim;
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
