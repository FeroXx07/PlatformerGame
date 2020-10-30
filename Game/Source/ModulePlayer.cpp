#include "ModulePlayer.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "ModuleCollisions.h"
#include "Audio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
//#include "ModuleHud.h"
#include "DeathScene.h"

#include <stdio.h>
#include "SDL/include/SDL_scancode.h"


//Now temporally is this
#define VELOCITY 20.0f
#define MAXVELOCITY_X 200.0f
#define MAXVELOCITY_Y 300.0f // Before it was 1000.0f

ModulePlayer::ModulePlayer(bool b) : Module(b)
{
	name.Create("player");

	// left idle animation
	/*leftIdleAnim.PushBack({ 66, 24, 12, 16 });
	leftIdleAnim.loop = true;
	leftIdleAnim.speed = 0.1f;*/

	// right idle animation
	rightIdleAnim.PushBack({ 0, 0, 94, 160 });
	rightIdleAnim.PushBack({ 94, 0, 94, 160 });
	rightIdleAnim.PushBack({ 189, 0, 95, 160 });
	rightIdleAnim.PushBack({ 284, 0, 95, 160 });
	rightIdleAnim.PushBack({ 379, 0, 95, 160 });
	rightIdleAnim.PushBack({ 474, 0, 95, 160 });
	rightIdleAnim.loop = true;
	rightIdleAnim.speed = 0.1f;

	leftIdleAnim.PushBack({ 0, 163, 92, 157 });
	leftIdleAnim.PushBack({ 95, 164, 92, 157 });
	leftIdleAnim.PushBack({ 190, 165, 92, 157 });
	leftIdleAnim.PushBack({ 284, 165, 92, 157 });
	leftIdleAnim.PushBack({ 380, 162, 92, 157 });
	leftIdleAnim.PushBack({ 475, 161, 92, 157 });
	leftIdleAnim.loop = true;
	leftIdleAnim.speed = 0.1f;

	rightRunAnim.PushBack({ 126, 329, 106, 146 });
	rightRunAnim.PushBack({ 843, 331, 132, 156 });
	rightRunAnim.PushBack({ 724, 325, 118, 157 });
	rightRunAnim.PushBack({ 615, 328, 107, 146 });
	rightRunAnim.PushBack({ 482, 326, 131, 152 });
	rightRunAnim.PushBack({ 341, 327, 140, 149 });
	rightRunAnim.PushBack({ 233, 330, 106, 154 });
	rightRunAnim.PushBack({ 0, 322, 122, 159 }); 
	rightRunAnim.loop = true;
	rightRunAnim.speed = 0.1f;

	leftRunAnim.PushBack({ 0, 502, 122, 154 }); // movement left 1
	leftRunAnim.PushBack({ 125, 496, 118, 158 }); // idle left
	leftRunAnim.PushBack({ 252, 506, 108, 148 }); // movement left 2
	leftRunAnim.PushBack({ 369, 500, 132, 150 }); // idle left
	leftRunAnim.PushBack({ 512, 505, 138, 150 });
	leftRunAnim.PushBack({ 666, 505, 108, 155 });
	leftRunAnim.PushBack({ 790, 505, 106, 147 });
	leftRunAnim.PushBack({ 913, 495, 122, 154 });
	leftRunAnim.loop = true;
	leftRunAnim.speed = 0.1f;

	jumpRightAnim.PushBack({ 0,0,112,145 });
	jumpRightAnim.PushBack({ 112,0,93,169 });
	jumpRightAnim.PushBack({ 205,0,136,149 });
	jumpRightAnim.PushBack({ 341,0,138,165 });
	jumpRightAnim.loop = false;
	jumpRightAnim.pingpong = false;
	jumpRightAnim.speed = 0.1f;

	jumpLeftAnim.PushBack({ 368,169,112,145 });
	jumpLeftAnim.PushBack({ 274,169,93,169 });
	jumpLeftAnim.PushBack({ 138,169,136,149 });
	jumpLeftAnim.PushBack({ 0,169,138,164 });
	jumpLeftAnim.loop = false;
	jumpLeftAnim.pingpong = false;
	jumpLeftAnim.speed = 0.1f;
}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = app->tex->Load("Assets/textures/AnimIdle.png"); 
	jumpTexture = app->tex->Load("Assets/textures/AnimJump.png");;

	//Starting position of the Mario
	playerPos = { -1000.0f,-1000.0f };
	playerWH = { 95.0f,160.0f };
	playerCollider = app->collisions->AddCollider({(int)playerPos.x + (int)playerWH.x / 2,(int)playerPos.y,(int)playerWH.x/2,(int)playerWH.y}, Collider::Type::PLAYER, app->player);
	

	currentAnimation = &rightIdleAnim;
	currentTexture = &texture;
	/*FX_Walking =*/ /*app->audio->LoadFx("Assets/Music/SFX_Walking.wav");*/
	/*++activeFx; ++totalFx;

	frameCountWalking = 0;
	conveyorCounter = 0;*/

	return ret;
}

bool ModulePlayer::Update(float dt)
{
	bool ret = true;

	Input();

	Logic(dt);

	if (currentAnimation != NULL)
	{
		currentAnimation->Update();
	}

	return ret;
}

void ModulePlayer::Input()
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		// Controlling player movement based on if they are on the ground or air.
		velocity.x += -VELOCITY;

		if (walkingFX == false) walkingFX = app->audio->PlayFx(FX_Walking);
		if (frameCountWalking == 11)
		{
			walkingFX = false;
			frameCountWalking = 0;
		}
		++frameCountWalking;

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
		velocity.x += VELOCITY;

		if (walkingFX == false) walkingFX = app->audio->PlayFx(FX_Walking);
		if (frameCountWalking == 11)
		{
			walkingFX = false;
			frameCountWalking = 0;
		}
		++frameCountWalking;

		if (currentAnimation != &rightRunAnim)
		{
			rightRunAnim.Reset();
			currentAnimation = &rightRunAnim;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		velocity.y += VELOCITY;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		velocity.y -= VELOCITY;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isGround == true)
	{
		// Stop moving just before jumping
		velocity.x = velocity.x / 2;
		if (velocity.y == 0)
		{
			velocity.y = -160.0f * 2;
			isAir = true;
		}
		velocity.y = -160.0f * 2;
		isAir = true;
		jumpRightAnim.Reset();
		jumpLeftAnim.Reset();
	}

	// If last movement was left, set the current animation back to left idle
	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_UP)
	{
		currentAnimation = &leftIdleAnim;
	}

	// If last movement was right, set the current animation back to left idle
	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_UP)
	{
		currentAnimation = &rightIdleAnim;
	}

	// Debug Keys

	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		isDebug = !isDebug;
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{

	}
}

void ModulePlayer::Logic(float dt)
{
	// Gravity
	if (isAir || collisionExist == false)
	{
		isGround = false;

		currentTexture = &jumpTexture;
		/*currentAnimation = &jumpRightAnim;*/

		if (velocity.x >= 0)
		{
			currentAnimation = &jumpRightAnim;
		}
		else
		{
			currentAnimation = &jumpLeftAnim;
		}

		if (velocity.y <= -200)
		{
			//velocity.y += 100.0f/2 * dt;
			velocity.y += 100.0f * 4.0f * dt;
		}
		else 
		{
			velocity.y += 100.0f * 3.0f * dt;
		}

		if (velocity.x) // Make player lose some velocity in x while is in air
		{
			velocity.x += -5.0f * velocity.x * dt; // Resistence/Friction in the air
		}
	}

	if (isGround) // Stopping the player gradually while at ground
	{
		currentTexture = &texture;
		if (currentAnimation == &jumpRightAnim)
		{
			currentAnimation = &rightIdleAnim;
		}
		if (isAir == false)
		{
			velocity.y = 0;
		}
		velocity.x += -1.0f * velocity.x * dt; // Resistence/Friction in the ground
		if (fabs(velocity.x) < 0.01f) // Stop the player once velocity is too small
			velocity.x = 0;
	}

	printf("Velocity in X = %f\nVelocity in Y = %f\n\n", velocity.x, velocity.y);
	printf("Position in X = %f\nPosition in Y = %f\n\n", playerPos.x, playerPos.y);

	// Integrators
	playerPos.x = playerPos.x + velocity.x * dt;
	playerPos.y = playerPos.y + velocity.y * dt;

	playerCollider->SetPos(playerPos.x + (int)playerWH.x / 4, playerPos.y);

	//The camera follows player(at the center)
	if (cameraFollow)
	{
		app->render->camera.x = app->render->camera.w / 2 - playerPos.x - playerWH.x;
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

	printf("Ground = %s\n", isGround ? "true" : "false");
	printf("Air = %s\n", isAir ? "true" : "false");
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
			if ((playerCollider->rect.y + playerCollider->rect.h >= c2->rect.y) && (playerCollider->rect.y + playerCollider->rect.h <= c2->rect.y + 4))
			{
				playerPos.y = c2->rect.y - playerCollider->rect.h;
				isGround = true;
				isAir = false;
				LOG("Player feet on ground");
				ret = true;
			}
		}
	}
	else
	{
		isGround = false;
		isAir = true;
	}

	return ret;
}

bool ModulePlayer::CleanUp()
{
	//activeTextures = activeColliders = activeFonts = activeFx = 0;

	// TODO 1: Remove ALL remaining resources. Update resource count properly

	app->tex->UnLoad(texture);
	app->tex->UnLoad(jumpTexture);
	
	return true;
}