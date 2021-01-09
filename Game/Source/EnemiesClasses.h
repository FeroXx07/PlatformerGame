#include "Enemy.h"

class EnemyWalking : public Enemy
{
public:
	EnemyWalking();
	~EnemyWalking();
	bool Update(float dt);
	bool Draw();
	DirectionState Gps(iPoint& p);

	void SetVelDirection(DirectionState dir);

	int spawnDelay = 0;

	// The original spawning position. The wave will be calculated from that
	int spawnY = 0;

	// The enemy animation
	Animation anim;
	Animation deathAnim;
	int maxHealth = 300;

	//Pathfinding
	bool hasPath = false;
	DynArray<iPoint> path;
};

class EnemyFlying : public Enemy
{
	// Constructor (x y coordinates in the world)
	// Creates animation data and the collider
public:
	EnemyFlying();
	~EnemyFlying();

	bool Update(float dt);
	bool Draw();
	DirectionState Gps(iPoint& p);

	void SetVelDirection(DirectionState dir);

	int spawnDelay = 0;

	// The original spawning position. The wave will be calculated from that
	int spawnY = 0;

	// The enemy animation
	Animation anim;
	Animation deathAnim;
	int maxHealth = 300;

	//Pathfinding
	bool hasPath = false;
	DynArray<iPoint> path;
	Enemy::DirectionState vertical = STOP;
	float counterDoPath = 0.0f;
};

class ItemStar : public Enemy
{
public:

	// Constructor (x y coordinates in the world)
	// Creates animation data and the collider
	ItemStar();

	// The enemy is going to perform a sinusoidal movement
	bool Update(float dt);
	bool Draw();
private:

	int spawnDelay = 0;

	// The original spawning position. The wave will be calculated from that
	int spawnY = 0;

	// The enemy animation
	Animation anim;
};

class ItemHealth : public Enemy
{
public:

	// Constructor (x y coordinates in the world)
	// Creates animation data and the collider
	ItemHealth();

	// The enemy is going to perform a sinusoidal movement
	bool Update(float dt);
	bool Draw();

private:

	int spawnDelay = 0;

	// The original spawning position. The wave will be calculated from that
	int spawnY = 0;

	// The enemy animation
	Animation anim;
};