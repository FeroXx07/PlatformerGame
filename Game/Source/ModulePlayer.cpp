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

//Previously was this
//#define JUMPSPEED 50.0f
//const float gravity = 60.0f + 50.0f

//Now temporally is this
#define SPEED 4
const float gravity = 1;         // pixels / second^2
const float deltaTime = 1.0f / 60.0f; // More or less 60 frames per second

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
	rightIdleAnim.speed = 0.01f;

	leftIdleAnim.PushBack({ 0, 163, 92, 157 });
	leftIdleAnim.PushBack({ 95, 164, 92, 157 });
	leftIdleAnim.PushBack({ 190, 165, 92, 157 });
	leftIdleAnim.PushBack({ 284, 165, 92, 157 });
	leftIdleAnim.PushBack({ 380, 162, 92, 157 });
	leftIdleAnim.PushBack({ 475, 161, 92, 157 });
	leftIdleAnim.loop = false;
	leftIdleAnim.speed = 0.01f;

	rightRunAnim.PushBack({ 0, 322, 122, 159 });
	rightRunAnim.PushBack({ 126, 329, 108, 146 });
	rightRunAnim.PushBack({ 231, 330, 106, 154 });
	rightRunAnim.PushBack({ 341, 327, 140, 149 });
	rightRunAnim.PushBack({ 482, 326, 131, 152 });
	rightRunAnim.PushBack({ 615, 328, 107, 146 });
	rightRunAnim.PushBack({ 724, 325, 118, 157 });
	rightRunAnim.PushBack({ 843, 331, 132, 156 });
	rightRunAnim.loop = false;
	rightRunAnim.speed = 0.01f;

	leftRunAnim.PushBack({ 0, 502, 122, 154 }); // movement left 1
	leftRunAnim.PushBack({ 125, 496, 118, 158 }); // idle left
	leftRunAnim.PushBack({ 252, 506, 108, 148 }); // movement left 2
	leftRunAnim.PushBack({ 369, 500, 132, 150 }); // idle left
	leftRunAnim.PushBack({ 512, 505, 138, 150 });
	leftRunAnim.PushBack({ 666, 505, 108, 155 });
	leftRunAnim.PushBack({ 790, 505, 106, 147 });
	leftRunAnim.PushBack({ 913, 495, 122, 154 });
	leftRunAnim.loop = false;
	leftRunAnim.speed = 0.01f;

	/*jumpAnim.PushBack({ 113, 24, 15, 15 });
	jumpAnim.loop = true;
	jumpAnim.speed = 0.1f;*/

	//climbingAnim.PushBack({ 138, 24, 13, 16 }); // climb movement 1
	//climbingAnim.PushBack({ 138, 40, 13, 16 }); // climb movement 2
	//climbingAnim.loop = true;
	//climbingAnim.speed = 0.1f;


	//climbedAnim.PushBack({ 161, 25, 14, 15 }); // when already up 1
	//climbedAnim.PushBack({ 184, 27, 16, 12 }); // when already up 2
	//climbedAnim.loop = false;
	//climbingAnim.speed = 0.1f;

	//sprite1Climbed.PushBack({ 184, 27 - 3, 16, 12 + 3 });
	//sprite1Climbed.loop = false;
	//sprite1Climbed.speed = 0.1f;

	//sprite2Climbed.PushBack({ 161, 25 - 3, 14, 15 + 3 });
	//sprite2Climbed.loop = false;
	//sprite2Climbed.speed = 0.1f;


	//idleClimbedAnim.PushBack({ 208, 25, 16, 15 }); // idle up
	//idleClimbedAnim.loop = false;
	//idleClimbedAnim.speed = 0.1f;

}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = app->tex->Load("Assets/textures/AnimIdle.png"); // arcade version
	

	//Starting position of the Mario
	position = { 10,10 };

	/*playerCollider = app->collisions->AddCollider({position.x,position.y,12,16}, Collider::Type::PLAYER, App->player);
	++activeColliders; ++totalColliders;*/

	currentAnimation = &rightIdleAnim; 

	/*FX_Walking =*/ app->audio->LoadFx("Assets/Music/SFX_Walking.wav");
	/*++activeFx; ++totalFx;

	frameCountWalking = 0;
	conveyorCounter = 0;*/

	return ret;
}

bool ModulePlayer::Update(float dt)
{
	bool ret = true;

	velocity.x = 0;
	velocity.y = 0;

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_DOWN)
	{
		velocity.x += 0.3f;

		leftIdleAnim.loop = true;
		currentAnimation = &leftIdleAnim;
		if (currentAnimation == &leftIdleAnim && (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT))
		{
			leftIdleAnim.loop = false;
			leftRunAnim.loop = true;
			currentAnimation = &leftRunAnim;
		}

		/*if (walkingFX == false) walkingFX = app->audio->PlayFx(FX_Walking);
		if (frameCountWalking == 11)
		{
			walkingFX = false;
			frameCountWalking = 0;
		}
		++frameCountWalking;*/
	}

	if ((app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT))
	{
		/*velocity.x += 0.3f;*/

		rightIdleAnim.loop = true;
		currentAnimation = &rightIdleAnim;

		/*if (walkingFX == false) walkingFX = app->audio->PlayFx(FX_Walking);
		if (frameCountWalking == 11) 
		{
			walkingFX = false;
			frameCountWalking = 0;
		}
		++frameCountWalking;*/
	}

	//if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN && isGround == true))
	//{
	//	// Jump here
	//}

	//// If last movement was left, set the current animation back to left idle
	//if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_UP)
	//{
	//	currentAnimation = &leftIdleAnim;
	//}
	//
	//// If last movement was right, set the current animation back to left idle
	//if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_UP)
	//{
	//	currentAnimation = &rightIdleAnim;
	//}
	
	// Debug Keys

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		currentAnimation = &rightIdleAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN )
	{
		
	}

	position.x = position.x + velocity.x * dt;
	position.y = position.y + velocity.y * dt;

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
		app->render->DrawTexture(texture, position.x, position.y, &rect);
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