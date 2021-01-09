#include "Hud.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Log.h"
#include "Input.h"
#include "LevelScene.h"
#include "Player.h"
#include "SceneManager.h"
#include "Font.h"

#include <stdio.h>
Hud::Hud() : Entity(EntityType::HUD)
{
	spriteStar = { 41, 0, 32, 30 };
	spriteAim = { 66, 129, 64, 64 };
	spriteLifes = { 74, 31, 24, 22 };
	spriteHearts = { 74, 100, 24, 22 };

	LOG("Loading hud");

	bool ret = true;

	char lookupTableNumbers[] = { "0123456789" };

	char lookupTableTextAndLives[] = { "0123456789/" };
	char lookupTableTextAndStars[] = { "0123456789/" };

	/*lifesFont = app->fonts->Load("Assets/Fonts/arcade_font.png", lookupTableTextAndLives, 1);
	starsFont = app->fonts->Load("Assets/Fonts/arcade_font_star.png", lookupTableTextAndStars, 1);*/

	itemsTexture = app->tex->Load("Assets/Common/spritesheet_items.png");
	aimTexture = app->tex->Load("Assets/Common/cursor_sprite.png");

	font = new Font("Assets/Fonts/pottaOneRegular.xml", app->tex);

	SDL_ShowCursor(SDL_DISABLE);
}

Hud::~Hud()
{
	app->tex->UnLoad(aimTexture);
	SDL_ShowCursor(SDL_ENABLE);
}

bool Hud::Update(float dt)
{
	// DrawDebug UI (score) --------------------------------------
	if (app->sceneManager->playerScene != nullptr)
	{
		sprintf_s(livesText, 5, "%0d", app->sceneManager->playerScene->lives);
		sprintf_s(starText, 5, "%0d", app->sceneManager->playerScene->stars);
		sprintf_s(healthText, 5, "%0d", app->sceneManager->playerScene->health);
	}
	return true;
}

bool Hud::Draw()
{
	/*app->fonts->BlitText(1050, 20, lifesFont, livesText);
	app->fonts->BlitText(600, 20, starsFont, starText);
	app->fonts->BlitText(1050, 80, lifesFont, healthText);*/
	app->render->DrawText(font, livesText, 1050, 20, 50, 0, { 255, 165, 0, 255 });
	app->render->DrawText(font, starText, 600, 20, 50, 0, { 255, 165, 0, 255 });
	app->render->DrawText(font, healthText, 1050, 80, 50, 0, { 255, 165, 0, 255 });

	app->render->DrawTexture(itemsTexture, 600 - 50, 20, &spriteStar, 0.0f); // 0.0f makes it fix to the screen
	app->render->DrawTexture(itemsTexture, 1050 - 50, 20, &spriteLifes, 0.0f);
	app->render->DrawTexture(itemsTexture, 1050 - 50, 80, &spriteHearts, 0.0f);

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	app->render->DrawTexture(aimTexture, mouseX, mouseY, &spriteAim, 0.0f);

	return true;
}

bool Hud::HandleInput(float dt)
{
	return true;
}
