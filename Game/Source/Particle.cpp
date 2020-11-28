#include "Particle.h"

#include "Collider.h"

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), speed(p.speed),
frameCount(p.frameCount), lifetime(p.lifetime)
{

}

Particle::~Particle()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}

bool Particle::Update(float dt)
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the delay has been reached
	if (!isAlive && frameCount >= 0)
		isAlive = true;

	if (isAlive)
	{
		anim.Update();

		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifetime > 0)
		{
			if (frameCount >= lifetime)
			{
				ret = false;
				collider->pendingToDelete = true;
			}
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (anim.HasFinished())
		{
			ret = false;
			collider->pendingToDelete = true;
		}

		// Update the position in the screen
		position.x += speed.x *dt;
		position.y += speed.y *dt;

		if (collider != nullptr)
			collider->SetPos(position.x, position.y);
	}

	return ret;
}