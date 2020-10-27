#include "ModulePlayer.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
//#include "ModuleCollisions.h"
#include "Audio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
//#include "ModuleHud.h"

#include <stdio.h>
#include "SDL/include/SDL_scancode.h"


//Now temporally is this
#define VELOCITY 100.0f
#define MAXVELOCITY_X 100.0f
#define MAXVELOCITY_Y 1000.0f

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

	rightRunAnim.PushBack({ 0, 322, 122, 159 });
	rightRunAnim.PushBack({ 126, 329, 108, 146 });
	rightRunAnim.PushBack({ 231, 330, 106, 154 });
	rightRunAnim.PushBack({ 341, 327, 140, 149 });
	rightRunAnim.PushBack({ 482, 326, 131, 152 });
	rightRunAnim.PushBack({ 615, 328, 107, 146 });
	rightRunAnim.PushBack({ 724, 325, 118, 157 });
	rightRunAnim.PushBack({ 843, 331, 132, 156 });
	rightRunAnim.loop = true;
	rightRunAnim.speed = 0.02f;

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

	jumpAnim.PushBack({ 0,0,112,145 });
	jumpAnim.PushBack({ 112,0,93,169 });
	jumpAnim.PushBack({ 205,0,136,149 });
	jumpAnim.PushBack({ 341,0,138,165 });
	jumpAnim.loop = false;
	jumpAnim.pingpong = false;
	jumpAnim.speed = 0.1f;

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
	position = { 10.0f,10.0f };
	playerWH = { 95.0f,160.0f };
	/*playerCollider = app->collisions->AddCollider({position.x,position.y,12,16}, Collider::Type::PLAYER, App->player);
	++activeColliders; ++totalColliders;*/

	currentAnimation = &rightIdleAnim;
	currentTexture = &texture;
	/*FX_Walking =*/ app->audio->LoadFx("Assets/Music/SFX_Walking.wav");
	/*++activeFx; ++totalFx;

	frameCountWalking = 0;
	conveyorCounter = 0;*/

	return ret;
}

bool ModulePlayer::Update(float dt)
{
	bool ret = true;

	//velocity.x = 0.0f;
	//velocity.y = 0.0f;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		// Controlling player movement based on if they are on the ground or air.
		velocity.x += (isGround ? -VELOCITY : -VELOCITY) * dt;

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
		velocity.x += (isGround ? VELOCITY : VELOCITY) * dt;

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
		velocity.x = velocity.x/2;
		if (velocity.y == 0)
		{
			velocity.y = -100.0f;
		}
		jumpAnim.Reset();
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

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		currentAnimation = &rightIdleAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{

	}

	// Gravity
	if (!isGround)
	{
		currentTexture = &jumpTexture;
		currentAnimation = &jumpAnim;
		// Controll gravity at start of the jump
		if (velocity.y < 0)
		{ 
			/*velocity.y += 4.0f * dt;*/
			
		}
		else // Controll gravity while falling after the jump
		{
			/*if ((velocity.y < 6.0f))
				velocity.y += 15.0f * dt;*/
			
		}
		velocity.y += 40.0f * dt;

		if (velocity.x) // Make player lose some velocity while is in air
		{
			velocity.x += -1.0f * velocity.x * dt;
		}
	}
	if (isGround) // Stopping the player gradually while at ground
	{
		currentTexture = &texture;
		if (currentAnimation == &jumpAnim)
		{
			currentAnimation = &rightIdleAnim;
		}
		velocity.x += -1.0f * velocity.x * dt;
		if (fabs(velocity.x) < 0.01f)
			velocity.x = 0;
	}

	printf("Velocity in X = %f\n Velocity in Y = %f\n\n", velocity.x, velocity.y);

	// Integrators
	position.x = position.x + velocity.x * dt;
	position.y = position.y + velocity.y * dt;

	// Limit max velocities
	if (velocity.x > MAXVELOCITY_X)
		velocity.x = MAXVELOCITY_X;
	if (velocity.x < -MAXVELOCITY_X)
		velocity.x = -MAXVELOCITY_X;

	if (velocity.y > MAXVELOCITY_Y)
		velocity.y = MAXVELOCITY_Y;
	if (velocity.y < -MAXVELOCITY_Y)
		velocity.y = -MAXVELOCITY_Y;

	// False camera ground
	if (position.y + playerWH.y >= app->render->camera.y + app->render->camera.h)
	{
		position.y = app->render->camera.y + app->render->camera.h - playerWH.y;
		velocity.y = 0;
		isGround = true;
	}
	else
		isGround = false;


	printf("Ground = %s\n", isGround ? "true" : "false");

	/*if (isGround)
	{
		velocity.y = 0;
	}*/

	if (currentAnimation != NULL)
	{
		currentAnimation->Update();
	}

	return ret;
}
bool ModulePlayer::PostUpdate()
{
	bool ret = true;

	if (currentAnimation != NULL)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(*currentTexture, position.x, position.y, &rect);
	}



	return ret;
}

//void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
//{
//	
//}

bool ModulePlayer::CleanUp()
{
	//activeTextures = activeColliders = activeFonts = activeFx = 0;

	// TODO 1: Remove ALL remaining resources. Update resource count properly

	app->tex->UnLoad(texture);
	
	
	return true;
}