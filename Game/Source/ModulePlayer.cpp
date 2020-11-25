#include "ModulePlayer.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "ModuleCollisions.h"
#include "Audio.h"
#include "ModuleFadeToBlack.h"
//#include "ModuleHud.h"
#include "DeathScene.h"
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

	texture = app->tex->Load("Assets/textures/blue_player_animations.png"); 
	jumpTexture = app->tex->Load("Assets/textures/blue_player_animations.png");
	dieTexture = app->tex->Load("Assets/textures/blue_player_animations.png");
	fallTexture = app->tex->Load("Assets/textures/blue_player_animations.png");
	
	//Starting position of the Mario
	playerWh = { 66.0f,79.0f };
	playerCollider = app->collisions->AddCollider({(int)playerPos.x + (int)playerWh.x / 2,(int)playerPos.y,(int)playerWh.x/2,(int)playerWh.y}, Collider::Type::PLAYER, (Module*)app->player);
	

	currentAnimation = &idleAnim;
	currentTexture = &texture;
	collisionExist = false;
	playerState = onAir;
	collisionFromBelow = false;
	godMode = false;
	return ret;
}

bool ModulePlayer::Update(float dt)
{
	bool ret = true;

	Input(dt);

	Logic(dt);
	Collisions(dt);

	if (velocity.y>=250.f/*dt*1000.0f > app->cappedMs*/)
	{
		float newDt = dt/25.0f;
		for (int i = 0; i < 5; ++i)
		{
			Logic(newDt);
			Collisions(newDt);
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
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godMode)
	{
		velocity.y += VELOCITY*dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godMode)
	{
		velocity.y -= VELOCITY*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && playerState == onGround)
	{
		// Stop moving just before jumping
		velocity.x = velocity.x / 2;
		if (velocity.y == 0)
		{
			velocity.y = -160.0f * 2;
			playerState = onAir;
			isJump = true;
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
}

void ModulePlayer::Logic(float dt)
{
	// Gravity
	if ( (playerState==onAir || collisionExist == false) && godMode == false && destroyed == false)
	{
		if (collisionExist == false)
			playerState = onAir;

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
			currentAnimation = &jumpAnim;
		}
		else if (velocity.y >= 0)
		{
			currentAnimation = &fallAnim;
		}

		if (velocity.x) // Make player lose some velocity in x while is in air
		{
			velocity.x += -0.5f * velocity.x * dt; // Resistence/Friction in the air
		}
	}

	if (playerState == onGround) // Stopping the player gradually while at ground
	{
		currentTexture = &texture;
		if (currentAnimation == &jumpAnim)
		{
			currentAnimation = &idleAnim;
		}
		else if (currentAnimation == &jumpAnim)
		{
			currentAnimation = &idleAnim;
		}
		if (playerState != onAir && godMode == false)
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

	printf("Velocity in X = %f\nVelocity in Y = %f\n\n", velocity.x, velocity.y);
	printf("Position in X = %f\nPosition in Y = %f\n\n", playerPos.x, playerPos.y);

	// Integrators
	playerPos.x = playerPos.x + velocity.x * dt;
	playerPos.y = playerPos.y + velocity.y * dt;

	playerCollider->SetPos(playerPos.x + (int)playerWh.x / 4, playerPos.y);

	//The camera follows player(at the center)
	if (destroyed == false)
	{
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

bool ModulePlayer::Collisions(float dt)
{
	bool ret = true;
	collisionExist = false;
	//Check manually all collisions with player
	//
	ListItem<Collider*>* listColliders;
	
	for (listColliders = app->collisions->colliders.start; listColliders != NULL; listColliders = listColliders->next)
	{
		if (playerCollider->Intersects(listColliders->data->rect))
			collisionExist = this->OnCollision(playerCollider, listColliders->data);
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
			LOG("Player through the ground");
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
				playerState = onGround;
				if (playerState == onAir)
				{
					//isAir = false;
					currentAnimation = &idleAnim;
				}
				isJump = false;
				LOG("Player feet on ground");
				ret = true;
			}
		}
	}
	else
	{
		playerState = onAir;
	}

	if (c2->type == Collider::Type::CHECKPOINT)
	{
		app->SaveGameRequest();
	}

	if (c2->type == Collider::Type::ENEMY && godMode == false)
		destroyed = true;

	if (c2->type == Collider::Type::WIN)
		win = true;

	return ret;
}

bool ModulePlayer::LoadState(pugi::xml_node& data)
{
	//...
	LOG("Loading Player state...");
	playerPos.x = data.child("position").attribute("x").as_int();
	playerPos.y = data.child("position").attribute("y").as_int();
	LOG("Player state succesfully loaded.\n Pos.x = %d Pos.y = %d", playerPos.x, playerPos.y);
	return true;
}


bool ModulePlayer::SaveState(pugi::xml_node& data) const
{
	//...
	// Delete old data
	data.remove_child("position");
	// Add new data
	LOG("Saving Player state...");
	pugi::xml_node pos = data.append_child("position");
	pos.append_attribute("x").set_value(playerPos.x);
	pos.append_attribute("y").set_value(playerPos.y);
	LOG("Player state succesfully saved. \n Pos.x = %d Pos.y = %d", playerPos.x, playerPos.y);
	return true;
}

bool ModulePlayer::CleanUp()
{
	//activeTextures = activeColliders = activeFonts = activeFx = 0;

	// TODO 1: Remove ALL remaining resources. Update resource count properly

	app->tex->UnLoad(texture);
	app->tex->UnLoad(jumpTexture);
	
	return true;
}