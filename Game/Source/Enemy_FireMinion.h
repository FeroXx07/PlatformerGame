#include "Entity.h"

class TestEntityEnemy : public Entity
{
public:

	// Constructor (x y coordinates in the world)
	// Creates animation data and the collider
	TestEntityEnemy(int x, int y);

	// The enemy is going to perform a sinusoidal movement
	void Update() override;

private:

	int spawnDelay = 0;


	// The original spawning position. The wave will be calculated from that
	int spawnY = 0;

	// The enemy animation
	Animation testAnimation;
	
	int enemyFireMinionSpeed = 1;
};

