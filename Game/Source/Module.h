#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;

class Module
{
public:

	Module(bool startEnabled = false) : active(false) 
	{
		isEnabled = startEnabled;
	}
	
	void Init()
	{
		active = true;
	}

	// Called before render is available
	// TODO 5: Sending config file to all modules
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		active = false;
		isEnabled = false;
		return true;
	}

	// Switches isEnabled and calls Start() method
	void Enable()
	{
		if (!isEnabled)
		{
			isEnabled = true;
			Start();
		}
	}

	// Switches isEnabled and calls CleanUp() method
	void Disable()
	{
		if (isEnabled)
		{
			isEnabled = false;
			CleanUp();
		}
	}

	inline bool IsEnabled() const { return isEnabled; }

public:

	SString name;
	bool active;

private:
	bool isEnabled = true;
};

#endif // __MODULE_H__