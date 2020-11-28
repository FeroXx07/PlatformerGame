#ifndef __MODULE_PARTICLES_H__
#define __MODULE_PARTICLES_H__

#include "Module.h"

#include "Particle.h"
#include "Collider.h"
#include "List.h"

struct SDL_Texture;
struct Collider;

class ModuleParticles : public Module
{
public:
	// Constructor
	// Initializes all the particles in the array to nullptr
	ModuleParticles(bool startEnabled);

	//Destructor
	~ModuleParticles();

	// Called when the module is activated
	// Loads the necessary textures for the particles
	bool Start() override;

	bool PreUpdate() override;

	// Called at the middle of the application loop
	// Iterates all the particles and calls its Update()
	// Removes any "dead" particles
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Iterates all the particles and draws them
	bool PostUpdate() override;

	// Called on application exit
	// Destroys all active particles left in the array
	bool CleanUp() override;

	bool OnCollision(Collider* c1, Collider* c2);

	// Creates a new particle and adds it to the array
	// Param particle	- A template particle from which the new particle will be created
	// Param x, y		- Position x,y in the screen (upper left axis)
	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
	void AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType = Collider::Type::TYPE_NONE, uint delay = 0);

	inline uint GetParticlesCount() const { return particles.Count(); };

private:
	// Particles spritesheet loaded into an SDL Texture
	SDL_Texture* texture = nullptr;

	// An array to store and handle all the particles
	//Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };
	List<Particle*>particles;
	// Total amount of particles laoded into the array
	uint particlesCount = 0;

public:
	//Template particle
	Particle bullet;
};

#endif // !__MODULEPARTICLES_H__