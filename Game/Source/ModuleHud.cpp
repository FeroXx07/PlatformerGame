#include "ModuleHud.h"

#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "Log.h"

#include <stdio.h>
ModuleHud::ModuleHud( bool start_enabled) : Module(start_enabled)
{

}


// Destructor
ModuleHud::~ModuleHud()
{
	this->CleanUp();
}


bool ModuleHud::Start()
{
	LOG("Loading hud");

	bool ret = true;

	char lookupTableNumbers[] = { "0123456789" };

	char lookupTableTextAndLives[] = { "0123456789/" };
	char lookupTableTextAndStars[] = { "0123456789/" };
	lifesFont = app->fonts->Load("Assets/Fonts/arcade_font.png", lookupTableTextAndLives, 1);
	starsFont = app->fonts->Load("Assets/Fonts/arcade_font_star.png", lookupTableTextAndStars, 1);

	return ret;
}

bool ModuleHud::Update(float dt)
{
	bool ret = true;
	// Draw UI (score) --------------------------------------
	sprintf_s(livesText, 5, "%4d", app->player->lives);
	sprintf_s(starText, 5, "%0d", app->player->lives);

	return ret;
}

bool ModuleHud::PostUpdate()
{
	bool ret = true;
	// TODO 3: Blit the text of the score in at the bottom of the screen
	app->fonts->BlitText(1050, 20, lifesFont, livesText);
	app->fonts->BlitText(600, 20, starsFont, starText);

		/*if (App->player->destroyed && App->hud->lives == 0 && App->intro->IsEnabled() == false)
		{
			App->fonts->BlitText(78, 185, lightBlueFont, "GAME");
			App->fonts->BlitText(120, 185, lightBlueFont, "OVER");
		}*/

	return ret;
}


bool ModuleHud::CleanUp()
{
	return true;
}
