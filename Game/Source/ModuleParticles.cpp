#include "ModuleParticles.h"

#include "App.h"

#include "Textures.h"
#include "Render.h"
#include "ModuleCollisions.h"
#include "Window.h"
#include "ModulePlayer.h"

#include "SDL/include/SDL_timer.h"
#include "Log.h"

ModuleParticles::ModuleParticles(bool startEnabled) : Module(startEnabled)
{
	/*for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;*/
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	texture = app->tex->Load("Assets/Common/spritesheet_items.png");
	// Do pushbacks of particle animations:
	// Particle->Anim->Pushback... etx
	bullet.anim.PushBack({ 74,77,24,22});
	bullet.anim.loop = false;
	bullet.anim.speed = 0.1f;

	//bullet.speed.x = app->player->velocity.x + 500.0f;
	bullet.lifetime = 60;
	return true;
}

bool ModuleParticles::CleanUp()
{
	app->tex->UnLoad(texture);
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	ListItem<Particle*>* list;
	list = particles.start;
	for (int i = 0; i < particles.Count(); ++i)
	{
		if (list != NULL)
			particles.Del(list);
		list = list->next;
	}
	particles.Clear();
	return true;
}

bool ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = true;
	//for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	//{
	//	// Always destroy particles that collide
	//	if (particles[i] != nullptr && particles[i]->collider == c1 && particles[i]->collider->type != Collider::Type::WALL) // Will not destroy walls
	//	{
	//		delete particles[i];
	//		particles[i] = nullptr;
	//		break;
	//	}
	//}

	ListItem<Particle*>* list;
	list = particles.start;
	for (int i = 0; i < particles.Count(); ++i)
	{
		if (list != NULL)
		{
			 //always destroy particles that collide
			if (list != nullptr && list->data->collider == c1) // will not destroy walls
			{
				if (c2->type != Collider::Type::PLAYER)
				{
					// If you don't want to destroy something:
					switch (list->data->collider->type)
					{
					case Collider::Type::BULLET:

						break;

					default:
						//particles.Del(list);
						break;
					}
					break;
				}
			}
		}
		list = list->next;
	}
	return ret;
}

bool ModuleParticles::PreUpdate()
{
	bool ret = true;

	ListItem<Collider*>* listEntities;
	ListItem<Particle*>* listParticles;

	for (listParticles = particles.start; listParticles != NULL; listParticles = listParticles->next)
	{
		// Check particles collision with entities
		for (listEntities = app->collisions->colliders.start; listEntities != NULL; listEntities = listEntities->next)
		{
			if (listParticles->data->collider->Intersects(listEntities->data->rect))
			{
				if (listEntities->data->listener!=nullptr)listEntities->data->listener->OnCollision(listEntities->data, listParticles->data->collider);
				listParticles->data->collider->listener->OnCollision(listParticles->data->collider, listEntities->data);
			}
		}
	}

	return ret;
}

bool ModuleParticles::Update(float dt)
{
	bool ret = true;

	ListItem<Particle*>* list;
	for (list = particles.start; list != NULL; list = list->next)
	{
		if (list->data->Update(dt) == false)
			particles.Del(list);
	}

	return ret;
}

bool ModuleParticles::PostUpdate()
{
	bool ret = true;
	//Iterating all particle array and drawing any active particles
	ListItem<Particle*>* list;
	for (list = particles.start; list != NULL; list = list->next)
	{
		if (list->data->isAlive)
			app->render->DrawTexture(texture, list->data->position.x, list->data->position.y, &(list->data->anim.GetCurrentFrame()));
	}

	return ret;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
{
	
	Particle* p = new Particle(particle);
	p->frameCount = -(int)delay;
	p->position.x = x;
	p->position.y = y;

	if (colliderType != Collider::Type::TYPE_NONE)
		p->collider = app->collisions->AddCollider(p->anim.GetCurrentFrame(), colliderType, this);

	particles.Add(p);
}