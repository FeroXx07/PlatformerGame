#include "ModuleDebugInfo.h"

#include "App.h"
#include "Globals.h"

#include "ModuleFonts.h"
#include "Input.h"
//#include "ModuleCollisions.h"
#include "Textures.h"
#include "ModuleParticles.h"
#include "Audio.h"

#include "SDL/include/SDL.h"
#include <stdio.h>

ModuleDebugInfo::ModuleDebugInfo(bool startEnabled) : Module()
{
	name.Create("debug");
}

ModuleDebugInfo::~ModuleDebugInfo()
{

}

bool ModuleDebugInfo::Start()
{
	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	debugFont = app->fonts->Load("Assets/Fonts/rtype_font.png", lookupTable, 1);
	
	return true;
}

bool ModuleDebugInfo::CleanUp()
{
	//TODO: Unload debug font!

	return true;
}

bool ModuleDebugInfo::Update()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		debugMemLeaks = !debugMemLeaks;

	/*if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		inspectedModule = (Module*)app->player;

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		inspectedModule = (Module*)app->hammer;

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		inspectedModule = (Module*)app->level4;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		inspectedModule = (Module*)app->intro;

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		inspectedModule = (Module*)app->level4win;*/

	if (app->input->GetKey(SDL_SCANCODE_F12) == KEY_DOWN)
		inspectedModule = nullptr;


	return ret;
}

bool ModuleDebugInfo::PostUpdate()
{
	bool ret = true;

	if (!debugMemLeaks)
	{
		//App->fonts->BlitText(10, 1, debugFont, "press f2 to open mem leaks debug info");
	}
	else
	{
		app->fonts->BlitText(10, 1, debugFont, "press f2 to close mem leaks debug info");

		app->fonts->BlitText(10, 20, debugFont, "total loaded resources");

		// Display total textures loaded
		sprintf_s(debugText, 150, "textures  %i", app->tex->textures.count());
		app->fonts->BlitText(20, 35, debugFont, debugText);

		// Display total audio files loaded
		sprintf_s(debugText, 150, "audio fx  %i", app->audio->fx.count());
		app->fonts->BlitText(20, 50, debugFont, debugText);

		// Display total font files loaded
		sprintf_s(debugText, 150, "fonts     %i", app->fonts->GetFontsCount());
		app->fonts->BlitText(20, 65, debugFont, debugText);

		//// Display total colliders loaded
		//sprintf_s(debugText, 150, "colliders %i", app->collisions->GetColliderCount());
		//app->fonts->BlitText(20, 80, debugFont, debugText);

		//// Display total particles loaded
		//sprintf_s(debugText, 150, "particles %i", app->particles->GetParticlesCount());
		//app->fonts->BlitText(20, 95, debugFont, debugText);

		app->fonts->BlitText(10, 120, debugFont, "press f7 to f10 to display specific modules");

		if (inspectedModule != nullptr)
		{
			DrawModuleResources(inspectedModule);
		}
	}

	return ret;
}

void ModuleDebugInfo::DrawModuleResources(Module* module)
{
	////sprintf_s(debugText, 150, "module %s", module->name);
	//app->fonts->BlitText(20, 140, debugFont, debugText);

	//sprintf_s(debugText, 150, "active total");
	//app->fonts->BlitText(155-60, 140, debugFont, debugText);

	//sprintf_s(debugText, 150, "textures     %i  %i", module->activeTextures, module->totalTextures);
	//app->fonts->BlitText(30, 155, debugFont, debugText);

	//sprintf_s(debugText, 150, "audio fx     %i  %i", module->activeFx, module->totalFx);
	//app->fonts->BlitText(30, 170, debugFont, debugText);

	//sprintf_s(debugText, 150, "fonts        %i  %i", module->activeFonts, module->totalFonts);
	//app->fonts->BlitText(30, 185, debugFont, debugText);

	//sprintf_s(debugText, 150, "colliders    %i  %i", module->activeColliders, module->totalColliders);
	//app->fonts->BlitText(30, 200, debugFont, debugText);
}