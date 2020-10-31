#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)


struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module
{
public:
	// Constructor
	ModulePlayer(bool b);

	// Destructor
	~ModulePlayer();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start();

	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt);

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate() ;

	// Collision callback, called when the player intersects with another collider
	
	/*void OnCollision(Collider* c1, Collider* c2) override;*/

	bool CleanUp();

public:
	// Position of the player in the map
	fPoint playerPos;
	fPoint playerWH; // Data that contains width and height of player
	fPoint colliderPos;

	// The speed in which we move the player (pixels per frame)
	fPoint velocity = { 0,0 };

	SDL_Texture* texture = nullptr;
	Collider* playerCollider = nullptr;

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnimation = nullptr;
	Animation leftIdleAnim;
	Animation rightIdleAnim;
	Animation leftRunAnim;
	Animation rightRunAnim;
	Animation jumpRightAnim;
	Animation jumpLeftAnim;
	Animation dieAnimation;

	SDL_Texture** currentTexture = nullptr;
	SDL_Texture* jumpTexture = nullptr;
	SDL_Texture* dieTexture = nullptr;

	// Collision callback, called when the player intersects with another collider
	bool OnCollision(Collider* c1, Collider* c2);

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool cameraFollow = false;
	bool collisionExist = false;
	bool destroyed = false;
	bool win = false;
private:
	bool isGround = false;
	bool isAir = true;
	bool collisionFromBelow = false;
	bool godMode = false;

	void Input();
	void Logic(float dt);
};

#endif //!__MODULE_PLAYER_H__