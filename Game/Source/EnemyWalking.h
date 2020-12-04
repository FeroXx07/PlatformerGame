#include "Entity.h"
#include "DynArray.h"
class EnemyWalking : public Entity
{
public:

	// Constructor (x y coordinates in the world)
	// Creates animation data and the collider
	EnemyWalking(int x, int y);

	void Update(float dt) override;

	DirectionState Gps(iPoint& p);

	void SetVelDirection(DirectionState dir);
private:

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

