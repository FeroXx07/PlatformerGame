#include "ModuleCollisions.h"

#include "App.h"

#include "Render.h"
#include "Input.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#include "SDL/include/SDL_Scancode.h"
#include "ModulePlayer.h"

ModuleCollisions::ModuleCollisions(bool b) : Module(b)
{
	ListItem<Collider*>* colliders2;
	colliders2 = colliders.start;

	matrix[Collider::Type::GROUND][Collider::Type::GROUND] = false;
	matrix[Collider::Type::GROUND][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::GROUND][Collider::Type::DEATH] = false;
	matrix[Collider::Type::GROUND][Collider::Type::WIN] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::GROUND] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::DEATH] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::WIN] = true;

	matrix[Collider::Type::DEATH][Collider::Type::GROUND] = false;
	matrix[Collider::Type::DEATH][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::DEATH][Collider::Type::DEATH] = false;
	matrix[Collider::Type::DEATH][Collider::Type::WIN] = false;

	matrix[Collider::Type::WIN][Collider::Type::GROUND] = false;
	matrix[Collider::Type::WIN][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WIN][Collider::Type::DEATH] = false;
	matrix[Collider::Type::WIN][Collider::Type::WIN] = false;
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
	for (int i = 0 ; i < colliders.Count(); ++i)
	{
		if (listColl != NULL && listColl->data->pendingToDelete == true)
		{
			colliders.Del(listColl);
		}
		listColl = listColl->next;
	}

	//Collider* c1;
	//Collider* c2;

	//app->player->collisionExist = false;
	//
	//ListItem<Collider*>* list1;
	//list1 = colliders.start;
	//for(int i = 0; i < colliders.Count(); ++i)
	//{
	//	// skip empty colliders
	//	if (list1 == NULL)
	//	{
	//		list1 = list1->next;
	//		continue;
	//	}

	//	c1 = list1->data;

	//	ListItem<Collider*>* list2;
	//	list2 = colliders.start;
	//	// avoid checking collisions already checked
	//	for(int k = i+1; k < colliders.Count(); ++k)
	//	{
	//		// skip empty colliders
	//		if (list2 == NULL)
	//		{
	//			if (list2->next == NULL)
	//				break;
	//			list2 = list2->next;
	//			continue;
	//		}

	//		c2 = list2->data;

	//		if(c1->Intersects(c2->rect))
	//		{
	//			if (matrix[c1->type][c2->type] && c1->listener)
	//			{
	//				if (c1->type == Collider::Type::PLAYER)
	//				{
	//					app->player->collisionExist = c1->listener->OnCollision(c1, c2);
	//				}
	//				else
	//					c1->listener->OnCollision(c1, c2); //efecto de la colisión
	//			}
	//				
	//		
	//			if(matrix[c2->type][c1->type] && c2->listener) 
	//				c2->listener->OnCollision(c2, c1);
	//		}

	//		list2 = list2->next;
	//	}
	//	list1 = list1->next;
	//}

	return ret;
}

bool ModuleCollisions::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
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
	for(int i = 0; i < colliders.Count(); ++i)
	{
		if (list1 == NULL)
		{
			list1 = list1->next;
			continue;
		}
		
		switch(list1->data->type)
		{
			case Collider::Type::TYPE_NONE: // white
			app->render->DrawRectangle(list1->data->rect, 255, 255, 255, alpha);
			break;
			case Collider::Type::GROUND: // blue
			app->render->DrawRectangle(list1->data->rect, 255, 255, 0, alpha);
			break;
			case Collider::Type::PLAYER: // green
			app->render->DrawRectangle(list1->data->rect, 0, 255, 0, alpha);
			break;
			case Collider::Type::DEATH: // red
			app->render->DrawRectangle(list1->data->rect, 255, 0, 0, alpha);
			break;
			case Collider::Type::WIN: // pink
			app->render->DrawRectangle(list1->data->rect, 255, 192, 203, alpha);
			break;
			case Collider::Type::CHECKPOINT: // mediumspringgreen	
			app->render->DrawRectangle(list1->data->rect, 0, 250, 154, alpha);
			break;
			case Collider::Type::ITEM: // Olive	
			app->render->DrawRectangle(list1->data->rect, 128, 128, 0, alpha);
			break;
			case Collider::Type::ENEMY_HITBOX: // maroon	
			app->render->DrawRectangle(list1->data->rect, 128, 0, 0, alpha);
			break;
			case Collider::Type::ENEMY_HURTBOX: // Olive	
			app->render->DrawRectangle(list1->data->rect, 128, 128, 0, alpha);
			break;
			case Collider::Type::BULLET: // Hot pink	
			app->render->DrawRectangle(list1->data->rect, 255, 0, 208, alpha);
			break;
		}
		list1 = list1->next;
	}
}

// Called before quitting
bool ModuleCollisions::CleanUp()
{
	LOG("Freeing all colliders");

	ListItem<Collider*>* listColl;
	listColl = colliders.start;
	for (int i = 0; i < colliders.Count(); ++i)
	{
		if (listColl != NULL)
		{
			colliders.Del(listColl);
		}
		listColl = listColl->next;
	}
	colliders.Clear();
	return true;
}

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener, Collider::Items Item )
{
	Collider* ret = new Collider(rect, type, listener, Item);
	colliders.Add(ret);
	return ret;
}

void ModuleCollisions::RemoveCollider(Collider* collider)
{
	ListItem<Collider*>* listColl;
	listColl = colliders.start;
	for (int i = 0; i < colliders.Count(); ++i)
	{
		if (listColl != NULL && listColl->data == collider)
		{
			colliders.Del(listColl);
		}
		listColl = listColl->next;
	}
}