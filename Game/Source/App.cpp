#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "DeathScene.h"
#include "ModuleFadeToBlack.h"
#include "TitleScreen.h"
#include "ModulePlayer.h"
#include "ModuleHud.h"
#include "Map.h"
#include "ModuleInitialScreen.h"
#include "ModuleCollisions.h"
#include "WinScreen.h"

#include "ModuleEntities.h"
#include "ModuleParticles.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	dt = 1.0f / 60.0f;

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene(false);
	titleScreen = new TitleScreen(false);
	deathScene = new DeathScene(false);
	winScreen = new WinScreen(false);
	hud = new ModuleHud(false);
	player = new ModulePlayer(false);
	map = new Map();
	fade = new ModuleFadeToBlack();
	initialScreen = new ModuleInitialScreen(true);
	collisions = new ModuleCollisions(true);
	entities = new Entities(false);
	particles = new ModuleParticles(false);

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);

	AddModule(deathScene);
	AddModule(initialScreen);
	AddModule(map);

	AddModule(scene);
	AddModule(titleScreen);
	AddModule(winScreen);
	AddModule(player);
	AddModule(hud);

	AddModule(entities);
	AddModule(particles);
	AddModule(collisions);
	AddModule(fade);

	// render last to swap buffer
	AddModule(render);
	
	PERF_PEEK(ptimer);
}

// Destructor
App::~App()
{
	// release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());

        // L08: TODO 1: Read from config file your framerate cap
		newMaxFramerate = configApp.attribute("framerate_cap").as_int();
		if (!(newMaxFramerate <=0))
			cappedMs = (1000.0f / (float)newMaxFramerate);

	}

	if(ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// TODO 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}
	
	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(ptimer);
	
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		if (item->data->IsEnabled() == true)
		{
			ret = item->data->Start();
		}
		item = item->next;
	}
	
	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	//printf("Camera in X = %d\nCamera in Y = %d\n\n", app->render->camera.x, app->render->camera.y);

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
    frameCount++;
    lastSecFrameCount++;
	dt = frameTime.ReadSec();
	frameTime.Start();
    // L08: TODO 4: Calculate the dt: differential time since last frame
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L02: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();
    
    // L07: TODO 4: Framerate calculations
	// Amount of frames since startup
	// Amount of time since game start (use a low resolution timer)
	// Average FPS for the whole game life
	// Amount of ms took the last update
	// Amount of frames during the last second
    
	float averageFps = 0.0f;
	float secondsSinceStartup = 0.0f;
	uint32 lastFrameMs = 0;
	uint32 framesOnLastSec = 0;

	secondsSinceStartup = startupTime.ReadSec();
	averageFps = (float)frameCount / startupTime.ReadSec();
	lastFrameMs = frameTime.Read();

	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	framesOnLastSec = prevLastSecFrameCount;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
			  averageFps, lastFrameMs, framesOnLastSec, dt, secondsSinceStartup, frameCount);

	app->win->SetTitle(title);

    // L08: TODO 2: Use SDL_Delay to make sure you get your capped framerate
	if (cappedMs - lastFrameMs > 0.0f && cappedMs != -1)
	{
		// L08: TODO 3: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
		PERF_START(ptimer);
		SDL_Delay(cappedMs - lastFrameMs);
		PERF_PEEK(ptimer);
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false || (pModule->IsEnabled()==false)) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false || (pModule->IsEnabled() == false)) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if((pModule->active == false || (pModule->IsEnabled() == false))) {
			continue;
		}
		
		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

// ---------------------------------------
// L02: TODO 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = true;

	//...
	SString newName("save_game");
	newName += ".xml";
	pugi::xml_document docData;
	pugi::xml_node docNode;
	pugi::xml_parse_result result = docData.load_file(newName.GetString());

	// Check result for loading errors
	if (result == NULL)
	{
		LOG("Could not load map xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		LOG("Starting to LoadState of each Module");
		docNode = docData.child("game_state");

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// Create a node for each module and send it to their Load function
			ret = item->data->LoadState(docNode.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	loadGameRequested = false;

	return ret;
}

// L02: TODO 7: Implement the xml save method for current state
bool App::SaveGame() const
{
	bool ret = true;

	//...

	SString newName("save_game");
	newName += ".xml";
	pugi::xml_document docData;
	pugi::xml_node docNode;

	pugi::xml_parse_result result = docData.load_file(newName.GetString());

	// Check result for loading errors
	if (result == NULL)
	{
		LOG("Could not load map xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		LOG("Starting to SaveState of each Module");
		docNode = docData.child("game_state");

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// Create a node for each module and send it to their Save function
			ret = item->data->SaveState(docNode.child(item->data->name.GetString()));
			item = item->next;
		}
	}
	LOG("Saving file %s",newName.GetString());
	docData.save_file(newName.GetString());

	saveGameRequested = false;

	return ret;
}



