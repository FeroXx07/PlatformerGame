#include "Entity.h"
#include "Collider.h"

class Bullet : public Entity
{
public:
	Bullet();
	~Bullet();

	bool Update(float dt);
	bool Draw();
	bool HandleInput(float dt);

	void OnCollision(Collider* collider) override;

	Collider* collider = nullptr;
	Animation bulletAnim;
	fPoint speed = { 0.0f,0.0f };

	bool isAlive;
	bool hasDied;

	uint lifetime = 60;
	int frameCount = 0;

	uint destroyed;
};
