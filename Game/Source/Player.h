#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();

	bool Update(float dt) override;
	bool Draw() override;
	bool HandleInput(float dt) override;

	fPoint playerWh; // Data that contains width and height of player
	fPoint colliderPos;

	fPoint velocity = { 0,0 };

	SDL_Texture* itemsTexture = nullptr;
	Collider* playerCollider = nullptr;
	Collider* previousCollision = nullptr;

	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation leftRunAnim;
	Animation rightRunAnim;
	Animation jumpAnim;
	Animation dieAnimation;
	Animation fallAnim;

	SDL_Texture** currentTexture = nullptr;

	// Collision callback, called when the player intersects with another collider
	bool OnCollision(Collider* c1, Collider* c2);

	bool cameraFollow = false;
	bool collisionExist = false;
	bool collisionEnemy = false;
	bool destroyed = false;
	bool win = false;
private:
	enum PlayerState { ON_GROUND, ON_AIR };
	PlayerState playerState = ON_AIR;
	enum PlayerDirection { RIGHT, LEFT, UP, DOWN };
	PlayerDirection playerDirection;

	bool isHurt = false;
	bool isJump = false;
	bool collisionFromBelow = false;
	bool godMode = false;
	bool loadDeath = false;

	void Input(float dt);
	void Logic(float dt);
	bool CheckCollisions(float dt);

	void CheckPlayerState(float dt);
	void BulletLogic(float dt);

public:
	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	// Player loses life and it is tp to checpoint
	void PlayerDied();
	// Player has lost all lives
	void PlayerLost();

	uint lives;
	uint health;
	uint stars;

public:

	uint walkingSfx;
	uint jumpingSfx;
	uint shootingSfx;
	uint itemPickedFx;
	uint flagPickedFx;

	int isWalking = false;
	//float counterWalking = 0.0f;
	Timer counterWalking;
	Timer counterDamage;
};