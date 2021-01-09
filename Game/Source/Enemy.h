#include "Entity.h"
#include "DynArray.h"

class Enemy : public Entity
{
public:
	enum DirectionState { STOP, LEFT, RIGHT, UP, DOWN, };
	// Constructor
	// Saves the spawn position for later movement calculations
	Enemy();
	Enemy(EntityType t);
	// Destructor
	~Enemy();

	// Returns the enemy's collider
	const Collider* GetCollider() const;

	virtual void EnemyUpdate(float dt);
	virtual void EnemyDraw();

	// Collision response
	void OnCollision(Collider* collider) override;

	// Sets flag for deletion and for the collider aswell
	virtual void SetToDelete();

	bool CheckRectEqual(SDL_Rect &a, SDL_Rect &b);

public:

	// The enemy's itemsTexture
	SDL_Texture* texture = nullptr;
	SDL_Texture* debugTexture = nullptr;
	// Sound fx when destroyed
	int destroyedFx = 0;

	// A flag for the enemy removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

	SString name = SString("NONE");
	int health = 0;

	// The enemy's collider
	Collider* collider = nullptr;
protected:
	// A ptr to the previousScene animation
	Animation* currentAnim = nullptr;

	// Original spawn position. Stored for movement calculations
	fPoint spawnPos;
	fPoint entitySpeed = { 0,0 };

	DirectionState direction = STOP;
	float counter = 0;
	bool inCollision = false;

	fPoint drawOffset = { 0,0 };
	fPoint collOffset = { 0,0 };
};

