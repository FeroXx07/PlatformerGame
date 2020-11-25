#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"

class Module;

struct Collider
{
	enum Items
	{
		ITEM_NONE = -1,
	};

	enum Type
	{
		TYPE_NONE = -1,
		PLAYER,
		GROUND,
		ENEMY,
		WIN,
		CHECKPOINT,

		MAX=5
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr, Items item = Items::ITEM_NONE);

	void SetPos(int x, int y);

	bool Intersects(const SDL_Rect& r) const;

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listener = nullptr;
	Items item;
};


#endif // !__COLLIDER_H__

