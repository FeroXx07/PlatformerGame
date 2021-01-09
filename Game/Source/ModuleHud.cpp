#include "ModuleHud.h"

#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "Log.h"
#include "Input.h"

#include <stdio.h>
ModuleHud::ModuleHud( bool start_enabled) : Module(start_enabled)
{
	spriteStar = { 41, 0, 32, 30 };
	spriteAim = { 66, 129, 64, 64 };
	spriteLifes = { 74, 31, 24, 22 };
	spriteHearts = { 74, 100, 24, 22 };
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

	itemsTexture = app->tex->Load("Assets/Common/spritesheet_items.png");
	aimTexture = app->tex->Load("Assets/Common/cursor_sprite.png");
	SDL_ShowCursor(SDL_DISABLE);
	return ret;
}

bool ModuleHud::Update(float dt)
{
	bool ret = true;
	// Draw UI (score) --------------------------------------
	sprintf_s(livesText, 5, "%0d", app->player->lives);
	sprintf_s(starText, 5, "%0d", app->player->stars);
	sprintf_s(healthText, 5, "%0d", app->player->health);

	return ret;
}

bool ModuleHud::PostUpdate()
{
	bool ret = true;
	// TODO 3: Blit the text of the score in at the bottom of the screen
	app->fonts->BlitText(1050, 20, lifesFont, livesText);
	app->fonts->BlitText(600, 20, starsFont, starText);
	app->fonts->BlitText(1050, 80, lifesFont, healthText);

	app->render->DrawTexture(itemsTexture, 600 - 50, 20, &spriteStar,0.0f); // 0.0f makes it fix to the screen
	app->render->DrawTexture(itemsTexture, 1050 - 50, 20, &spriteLifes, 0.0f);
	app->render->DrawTexture(itemsTexture, 1050 - 50, 80, &spriteHearts, 0.0f);

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	app->render->DrawTexture(aimTexture, mouseX, mouseY, &spriteAim, 0.0f);
	return ret;
}


bool ModuleHud::CleanUp()
{
	SDL_ShowCursor(SDL_ENABLE);
	return true;
}
