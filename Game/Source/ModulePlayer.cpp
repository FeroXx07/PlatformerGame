#include "ModulePlayer.h"

#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "ModuleCollisions.h"
#include "Audio.h"
#include "ModuleFadeToBlack.h"
//#include "ModuleHud.h"
#include "DeathScene.h"
#include "ModuleParticles.h"
#include "Window.h"
#include "Map.h"

#include "Log.h"

#include <stdio.h>
#include "SDL/include/SDL_scancode.h"


//Now temporally is this
#define VELOCITY 200.0f
#define MAXVELOCITY_X 300.0f
#define MAXVELOCITY_Y 300.0f // Before it was 1000.0f

ModulePlayer::ModulePlayer(bool b) : Module(b)
{
	name.Create("player");

	// left idle animation
	/*leftIdleAnim.PushBack({ 66, 24, 12, 16 });
	leftIdleAnim.loop = true;
	leftIdleAnim.speed = 0.1f;*/

	// right idle animation
	idleAnim.PushBack({ 0, 298, 66, 80 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;

	rightRunAnim.PushBack({ 0, 0, 66, 79 });
	rightRunAnim.PushBack({ 73, 0, 66, 78 });
	rightRunAnim.PushBack({ 145, 0, 66, 75 });
	rightRunAnim.PushBack({ 219, 0, 94, 59 });
	rightRunAnim.PushBack({ 319, 0, 73, 66 });
	rightRunAnim.loop = true;
	rightRunAnim.speed = 0.15f;

	leftRunAnim.PushBack({ 327, 96, 66, 79 }); // movement left 1
	leftRunAnim.PushBack({ 255, 96, 66, 78 }); // idle left
	leftRunAnim.PushBack({ 181, 96, 66, 75 }); // movement left 2
	leftRunAnim.PushBack({ 79, 96, 94, 59 }); // idle left
	leftRunAnim.PushBack({ 0, 96, 73, 66 });
	leftRunAnim.loop = true;
	leftRunAnim.speed = 0.15f;

	jumpAnim.PushBack({ 0, 298, 66, 80 });
	jumpAnim.PushBack({ 81, 300, 67, 74 });
	jumpAnim.PushBack({ 165, 310, 64, 56 });
	jumpAnim.loop = false;
	jumpAnim.pingpong = false;
	jumpAnim.speed = 0.15f;

	dieAnimation.PushBack({ 264, 205, 63, 66 });
	dieAnimation.loop = false;
	dieAnimation.speed = 0.15f;

	fallAnim.PushBack({ 0,188,66,79 });
	fallAnim.loop = false;
	fallAnim.speed = 0.15f;

}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	itemsTexture = app->tex->Load("Assets/textures/blue_player_animations.png"); 
	jumpTexture = app->tex->Load("Assets/textures/blue_player_animations.png");
	dieTexture = app->tex->Load("Assets/textures/blue_player_animations.png");
	fallTexture = app->tex->Load("Assets/textures/blue_player_animations.png");

	// Audio of the player's actions

	walkingSfx = app->audio->LoadFx("Assets/Audio/Fx/player_walking.wav");
	jumpingSfx = app->audio->LoadFx("Assets/Audio/Fx/player_jump.wav");
	shootingSfx = app->audio->LoadFx("Assets/Audio/Fx/player_shot.wav");
	itemPickedFx = app->audio->LoadFx("Assets/Audio/Fx/item_taken.wav");
	flagPickedFx = app->audio->LoadFx("Assets/Audio/Fx/flag_taken.wav");

	//Starting position of the Mario
	playerWh = { 66.0f,79.0f };
	playerCollider = app->collisions->AddCollider({(int)playerPos.x + (int)playerWh.x / 2,(int)playerPos.y,(int)playerWh.x/2,(int)playerWh.y}, Collider::Type::PLAYER, (Module*)app->player);
	

	currentAnimation = &idleAnim;
	currentTexture = &itemsTexture;

	playerState = ON_AIR;
	collisionExist = false;
	collisionFromBelow = false;
	collisionEnemy = false;
	godMode = false;

	isWalking = false;

	return ret;
}

bool ModulePlayer::Update(float dt)
{
	bool ret = true;

	if (dt > 1.0f / 30.0f)
	{
		dt = 1.0f / 30.0f;
	}

	Input(dt);

	Logic(dt);
	CheckCollisions(dt);

	if (velocity.y>=250.f/*dt*1000.0f > app->cappedMs*/)
	{
		float newDt = dt/25.0f;
		for (int i = 0; i < 5; ++i)
		{
			Logic(newDt);
			CheckCollisions(newDt);
		}
		LOG("---------SUBSTEPPING---------");
	}

	if (currentAnimation != NULL)
	{
		currentAnimation->Update();
	}

	return ret;
}

void ModulePlayer::Input(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		// Controlling player movement based on if they are on the ground or air.
		velocity.x += -VELOCITY*1.5f*dt;

		if (currentAnimation != &leftRunAnim)
		{
			leftRunAnim.Reset();
			currentAnimation = &leftRunAnim;
		}
		if (playerState == ON_GROUND)
		{
			if (isWalking == false) 
				isWalking = app->audio->PlayFx(walkingSfx);
			if (counterWalking.Read() >= 20.0f)
			{
				isWalking = false;
			}
			counterWalking.Start();
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		// Controlling player movement based on if they are on the ground or air.
		//velocity.x += (isGround ? VELOCITY : VELOCITY) * dt;
		velocity.x += VELOCITY *1.5f*dt;

		if (currentAnimation != &rightRunAnim)
		{
			rightRunAnim.Reset();
			currentAnimation = &rightRunAnim;
		}
		if (playerState == ON_GROUND)
		{
			if (isWalking == false)
				isWalking = app->audio->PlayFx(walkingSfx);
			if (counterWalking.Read() >= 20.0f)
			{
				isWalking = false;
			}
			counterWalking.Start();
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode)
	{
		velocity.y += VELOCITY*dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godMode)
	{
		velocity.y -= VELOCITY*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && playerState == ON_GROUND)
	{
		// Stop moving just before jumping
		velocity.x = velocity.x / 2;
		if (velocity.y == 0)
		{
			velocity.y = -160.0f * 2;
			playerState = ON_AIR;
			isJump = true;
			app->audio->PlayFx(jumpingSfx);
		}
	/*	velocity.y = -160.0f * 2;
		isAir = true;*/
		jumpAnim.Reset();
	}

	// If last movement was left, set the current animation back to left idle
	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_UP)
	{
		currentAnimation = &idleAnim;
	}

	// If last movement was right, set the current animation back to left idle
	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_UP)
	{
		currentAnimation = &idleAnim;
	}

	// Debug Keys
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
	}

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		destroyed = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT)
	{
		// Teleport Checkpoint
		Collider* dstCheckpoint = NULL;
		for (int i = 0; i < app->map->checkpointsList.Count(); ++i)
		{
			// SDL_SCANCODE_0 is 39 SDL_SCANCODE_1 is 30 SDL_SCANCODE_2 is 31
			if(app->input->GetKey(30+i) == KEY_DOWN)
			{
				dstCheckpoint = app->map->checkpointsList.At(i)->data;
				break;
			}
		}

		if (dstCheckpoint != NULL)
		{
			playerPos = { (float)dstCheckpoint->rect.x,(float)dstCheckpoint->rect.y };
			velocity = { 0.0f,0.0f };
		}
	}

	BulletLogic(dt);
}

void ModulePlayer::BulletLogic(float dt)
{
	iPoint mousePoint;
	app->input->GetMousePosition(mousePoint.x, mousePoint.y);

	uPoint window;
	app->win->GetWindowSize(window.x, window.y);

	fPoint center = { (float)window.x / 2,(float)window.y / 2 };

	fPoint direction;
	direction = { center.x - mousePoint.x , center.y - mousePoint.y };

	float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);

	direction = { direction.x / magnitude, direction.y / magnitude }; // Vector unitario/dirección
	direction.Negate();

	printf("mouse = %f %f\n", center.x, center.y);
	printf("dir = %f %f\n", direction.x, direction.y);

	if ((app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN))
	{
		Particle newBullet = app->particles->bullet;
	
		newBullet.speed.y = { direction.y * 500.0f +app->player->velocity.y};
		newBullet.speed.x = { direction.x * 500.0f + app->player->velocity.x};

		app->particles->AddParticle(newBullet, playerPos.x + playerWh.x/2, playerPos.y, Collider::Type::BULLET);
		app->audio->PlayFx(shootingSfx);
	}
}

void ModulePlayer::Logic(float dt)
{
	CheckPlayerState(dt);

	if (health == 0)
		destroyed = true;

	printf("Velocity in X = %f\nVelocity in Y = %f\n\n", velocity.x, velocity.y);
	printf("Position in X = %f\nPosition in Y = %f\n\n", playerPos.x, playerPos.y);

	// Integrators
	if (destroyed == false)
	{
		playerPos.x = playerPos.x + velocity.x * dt;
		playerPos.y = playerPos.y + velocity.y * dt;

		playerCollider->SetPos(playerPos.x + (int)playerWh.x / 4, playerPos.y);

		//The camera follows player(at the center)
		app->render->camera.x = app->render->camera.w / 2 - playerPos.x - playerWh.x;
		app->render->camera.y = app->render->camera.h / 2 - playerPos.y;
	}

	// Limit max velocities
	if (velocity.x > MAXVELOCITY_X)
		velocity.x = MAXVELOCITY_X;
	if (velocity.x < -MAXVELOCITY_X)
		velocity.x = -MAXVELOCITY_X;

	if (velocity.y > MAXVELOCITY_Y)
		velocity.y = MAXVELOCITY_Y;
	if (velocity.y < -MAXVELOCITY_Y)
		velocity.y = -MAXVELOCITY_Y;

	/*printf("Ground = %s\n", isGround ? "true" : "false");
	printf("Air = %s\n", isAir ? "true" : "false");*/
	printf("Jump = %s\n", isJump ? "true" : "false");
}

void ModulePlayer::CheckPlayerState(float dt)
{

	// Gravity
	if ((playerState == ON_AIR || collisionExist == false)&& collisionEnemy == false && godMode == false && destroyed == false)
	{
		if (collisionExist == false)
			playerState = ON_AIR;

		currentTexture = &jumpTexture;
		/*currentAnimation = &jumpRightAnim;*/

		if (velocity.y <= -200)
		{
			//velocity.y += 100.0f/2 * dt;
			velocity.y += 100.0f * 4.0f * dt;
		}
		else
		{
			velocity.y += 100.0f * 3.0f * dt;
		}

		if (velocity.y < 0)
		{
			playerDirection = UP;
			currentAnimation = &jumpAnim;
		}
		else if (velocity.y >= 0)
		{
			playerDirection = DOWN;
			currentAnimation = &fallAnim;
		}

		if (velocity.x) // Make player lose some velocity in x while is in air
		{
			velocity.x += -0.8f * velocity.x * dt; // Resistence/Friction in the air
		}
	}

	if (playerState == ON_GROUND) // Stopping the player gradually while at ground
	{
		currentTexture = &itemsTexture;
		if (currentAnimation == &jumpAnim)
		{
			currentAnimation = &idleAnim;
		}
		else if (currentAnimation == &jumpAnim)
		{
			currentAnimation = &idleAnim;
		}
		if (playerState != ON_AIR && godMode == false)
		{
			velocity.y = 0;
		}
		velocity.x += -1.0f * velocity.x * dt; // Resistence/Friction in the ground
		if (fabs(velocity.x) < 0.01f) // Stop the player once velocity is too small
			velocity.x = 0;
	}

	if (destroyed)
	{
		currentAnimation = &dieAnimation;
		currentTexture = &dieTexture;
	}
}


bool ModulePlayer::PostUpdate()
{
	bool ret = true;

	if (currentAnimation != NULL)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(*currentTexture, playerPos.x, playerPos.y, &rect);
	}

	return ret;
}

bool ModulePlayer::CheckCollisions(float dt)
{
	bool ret = true;
	collisionExist = false;
	//Check manually all collisions with player
	//
	ListItem<Collider*>* listColliders;
	
	for (listColliders = app->collisions->colliders.start; listColliders != NULL; listColliders = listColliders->next)
	{
		if (playerCollider->Intersects(listColliders->data->rect))
		{
			if (previousCollision != listColliders->data && listColliders->data->type != Collider::Type::PLAYER)
				collisionExist = this->OnCollision(playerCollider, listColliders->data);
		}
	}

	return ret;
}

bool ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = false;

	if (c2->type == Collider::Type::GROUND)
	{
		//If player head enters the ground first collisionFromBelow = true
		//If collider of the ground is between the top of the head and the waist
		if (((playerCollider->rect.y < c2->rect.y) || (playerCollider->rect.y > c2->rect.y)) && (playerCollider->rect.y + playerCollider->rect.h > c2->rect.y +c2->rect.h/2))
		{
			collisionFromBelow = true;
			ret = false;
		}
		else
			collisionFromBelow = false;

		if (collisionFromBelow == false)
		{
			// Maintain the feet to the ground
			if ((playerCollider->rect.y + playerCollider->rect.h >= c2->rect.y) && (playerCollider->rect.y + playerCollider->rect.h <= c2->rect.y + c2->rect.h/2))
			{
				if (!(playerCollider->rect.y + playerCollider->rect.h <= c2->rect.y + 4))
					playerPos.y += -0.5f;
				else
					playerPos.y = c2->rect.y - playerCollider->rect.h;
				/*isGround = true;*/
				//playerState = ON_GROUND;
				if (playerState == ON_AIR)
				{
					//isAir = false;
					currentAnimation = &idleAnim;
				}
				playerState = ON_GROUND;
				isJump = false;
				ret = true;
			}
		}

		previousCollision = c2;
	}
	
	if (c2->type == Collider::Type::CHECKPOINT && previousCollision->type != Collider::Type::CHECKPOINT)
	{
		LOG("SAVING GAME");
		app->audio->PlayFx(flagPickedFx);
		app->SaveGameRequest();
		previousCollision = c2;
	}

	if (c2->type == Collider::Type::DEATH && previousCollision->type != Collider::Type::DEATH && godMode == false)
	{
		destroyed = true;
		previousCollision = c2;
	}

	if (c2->type == Collider::Type::ENEMY_HITBOX && previousCollision->type != Collider::Type::ENEMY_HITBOX && godMode == false)
	{
		health -= 1;
		previousCollision = c2;
	}

	if (c2->type == Collider::Type::ITEM && previousCollision->type != Collider::Type::ITEM && godMode == false)
	{

		switch (c2->item)
		{
		case Collider::Items::HEALTH:
			health += 1;
			app->audio->PlayFx(itemPickedFx);
			previousCollision = c2;
			c2->listener->OnCollision(c2, c1);
			break;
		case Collider::Items::STAR:
			stars += 1;
			app->audio->PlayFx(itemPickedFx);
			previousCollision = c2;
			c2->listener->OnCollision(c2, c1);
			break;
		default:
			break;
		}
	}

	if (c2->type == Collider::Type::ENEMY_HURTBOX && previousCollision->type != Collider::Type::ENEMY_HURTBOX && godMode == false)
	{
		if (counterDamage.ReadSec()>2.0f)
			health += -1;
		collisionEnemy = true;
		previousCollision = c2;
		counterDamage.Start();
	}
	else
		collisionEnemy = false;

	if (c2->type == Collider::Type::WIN)
		win = true;

	return ret;
}

bool ModulePlayer::LoadState(pugi::xml_node& data)
{
	//...
	LOG("Loading Player state...");
	playerPos.x = data.child("data").attribute("x").as_int();
	playerPos.y = data.child("data").attribute("y").as_int();
	if (loadDeath)
		loadDeath = false;
	else
	{
		lives = data.child("data").attribute("lives").as_int();
		health = data.child("data").attribute("health").as_int();
	}
	stars = data.child("data").attribute("stars").as_int();
	LOG("Player state succesfully loaded.\n Pos.x = %d Pos.y = %d", playerPos.x, playerPos.y);
	return true;
}


bool ModulePlayer::SaveState(pugi::xml_node& data) const
{
	//...
	// Delete old data
	data.remove_child("data");
	// Add new data
	LOG("Saving Player state...");
	pugi::xml_node pos = data.append_child("data");
	pos.append_attribute("x").set_value(playerPos.x);
	pos.append_attribute("y").set_value(playerPos.y);
	pos.append_attribute("lives").set_value(this->lives);
	pos.append_attribute("health").set_value(this->health);
	pos.append_attribute("stars").set_value(this->stars);
	LOG("Player state succesfully saved. \n Pos.x = %d Pos.y = %d", playerPos.x, playerPos.y);
	return true;
}

void ModulePlayer::PlayerDied()
{
	app->LoadGameRequest();
	app->player->destroyed = false;
	loadDeath = true;
	app->player->velocity.y = 0;
	app->player->velocity.x = 0;
	lives = lives - 1;
	health = 3;
}


bool ModulePlayer::CleanUp()
{
	//activeTextures = activeColliders = activeFonts = activeFx = 0;

	// TODO 1: Remove ALL remaining resources. Update resource count properly

	app->tex->UnLoad(itemsTexture);
	app->tex->UnLoad(jumpTexture);
	
	return true;
}