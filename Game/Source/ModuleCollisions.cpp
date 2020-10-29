#include "ModuleCollisions.h"

#include "App.h"

#include "Render.h"
#include "Input.h"

#include "SDL/include/SDL.h"
//#pragma comment( lib, "SDL/libx86/SDL2.lib")
//#pragma comment( lib, "SDL/libx86/SDL2main.lib")
#include "SDL/include/SDL_Scancode.h"


ModuleCollisions::ModuleCollisions(bool b) : Module(b)
{
	ListItem<Collider*>* colliders2;
	colliders2 = colliders.start;

	//for(uint i = 0; i < MAX_COLLIDERS; ++i)
	//	colliders[i] = nullptr;

	matrix[Collider::Type::GROUND][Collider::Type::GROUND] = false;
	matrix[Collider::Type::GROUND][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::GROUND][Collider::Type::ENEMY] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::GROUND] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;

	matrix[Collider::Type::ENEMY][Collider::Type::GROUND] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
	
}

// Destructor
ModuleCollisions::~ModuleCollisions()
{

}

bool ModuleCollisions::PreUpdate()
{
	bool ret = true;

	// Remove all colliders scheduled for deletion
	ListItem<Collider*>* listColl;
	listColl = colliders.start;
	for (int i = 0 ; i < colliders.count(); ++i)
	{
		while (listColl != NULL && listColl->data->pendingToDelete == true)
		{
			colliders.del(listColl);

		}
		listColl = listColl->next;
	}

	//// Remove all colliders scheduled for deletion
	//for(uint i = 0; i < MAX_COLLIDERS; ++i)
	//{
	//	if(colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
	//	{
	//		delete colliders[i];
	//		--colliderCount;
	//		colliders[i] = nullptr;
	//	}
	//}

	Collider* c1;
	Collider* c2;

	//for(uint i = 0; i < MAX_COLLIDERS; ++i)
	//{
	//	// skip empty colliders
	//	if(colliders[i] == nullptr)
	//		continue;

	//	c1 = colliders[i];

	//	// avoid checking collisions already checked
	//	for(uint k = i+1; k < MAX_COLLIDERS; ++k)
	//	{
	//		// skip empty colliders
	//		if(colliders[k] == nullptr)
	//			continue;

	//		c2 = colliders[k];

	//		if(c1->Intersects(c2->rect))
	//		{
	//			if(matrix[c1->type][c2->type] && c1->listener) 
	//				c1->listener->OnCollision(c1, c2); //efecto de la colisión
	//			
	//			if(matrix[c2->type][c1->type] && c2->listener) 
	//				c2->listener->OnCollision(c2, c1);
	//		}
	//	}
	//}

	ListItem<Collider*>* list1;
	list1 = colliders.start;
	for(int i = 0; i < colliders.count(); ++i)
	{
		// skip empty colliders
		if (list1 == NULL)
		{
			list1 = list1->next;
			continue;
		}

		c1 = list1->data;

		ListItem<Collider*>* list2;
		list2 = colliders.start;
		// avoid checking collisions already checked
		for(int k = i+1; k < colliders.count(); ++k)
		{
			// skip empty colliders
			if (list2 == NULL)
			{
				list2 = list2->next;
				continue;
			}

			c2 = list2->data;

			if(c1->Intersects(c2->rect))
			{
				if(matrix[c1->type][c2->type] && c1->listener) 
					c1->listener->OnCollision(c1, c2); //efecto de la colisión
				
				if(matrix[c2->type][c1->type] && c2->listener) 
					c2->listener->OnCollision(c2, c1);
			}

			list2 = list2->next;
		}
		list1 = list1->next;
	}

	return ret;
}

bool ModuleCollisions::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		debug = !debug;

	return ret;
}

bool ModuleCollisions::PostUpdate()
{
	bool ret = true;
	if (debug)
		DebugDraw();

	return ret;
}

void ModuleCollisions::DebugDraw()
{
	ListItem<Collider*>* list1;
	list1 = colliders.start;
	Uint8 alpha = 80;
	for(int i = 0; i < colliders.count(); ++i)
	{
		if (list1 == NULL)
		{
			list1 = list1->next;
			continue;
		}
		
		switch(list1->data->type)
		{
			case Collider::Type::NONE: // white
			app->render->DrawRectangle(list1->data->rect, 255, 255, 255, alpha);
			break;
			case Collider::Type::GROUND: // blue
			app->render->DrawRectangle(list1->data->rect, 255, 255, 0, alpha);
			break;
			case Collider::Type::PLAYER: // green
			app->render->DrawRectangle(list1->data->rect, 0, 255, 0, alpha);
			break;
			case Collider::Type::ENEMY: // red
			app->render->DrawRectangle(list1->data->rect, 0, 0, 255, alpha);
			break;
		}
		list1 = list1->next;
	}
}

// Called before quitting
bool ModuleCollisions::CleanUp()
{
	LOG("Freeing all colliders");

	/*for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}*/

	ListItem<Collider*>* listColl;
	listColl = colliders.start;
	for (int i = 0; i < colliders.count(); ++i)
	{
		if (listColl != NULL)
		{
			colliders.del(listColl);
		}
		listColl = listColl->next;
	}


	return true;
}

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener, Collider::Items Item )
{
	/*Collider* ret = nullptr;

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener, Item);
			++colliderCount;
			break;
		}
	}*/

	Collider* ret = new Collider(rect, type, listener, Item);
	colliders.add(ret);
	return ret;
}

void ModuleCollisions::RemoveCollider(Collider* collider)
{
	/*for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}*/
	ListItem<Collider*>* listColl;
	listColl = colliders.start;
	for (int i = 0; i < colliders.count(); ++i)
	{
		if (listColl != NULL && listColl->data == collider)
		{
			colliders.del(listColl);
		}
		listColl = listColl->next;
	}
}