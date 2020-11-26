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
	//char lookupTableTextAndLives[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ.-," }; // "," is LIVES SPRITE!!!!!!!!!
	//whiteFont = app->fonts->Load("pinball/fonts/score.png", lookupTableNumbers, 1);
	//redFont = app->fonts->Load("pinball/fonts/Lives_Font.png", lookupTableNumbers, 1);
	//greenFont = app->fonts->Load("pinball/fonts/previousScore.png", lookupTableNumbers, 1);
	//lightBlueFont = App->fonts->Load("Assets/Fonts/Fonts_LIGHTBLUE.png", lookupTableTextAndLives, 1);
	//darkBlueFont = App->fonts->Load("Assets/Fonts/Fonts_DARKBLUE.png", lookupTableNumbers, 1);
	score = 0;
	previousScore = 0;
	highScore = 0;
	return ret;
}

bool ModuleHud::Update()
{
	bool ret = true;
	// Draw UI (score) --------------------------------------
	sprintf_s(scoreText, 10, "%4d", score);
	sprintf_s(highScoreText, 10, "%4d", highScore);
	sprintf_s(previousScoreText, 10, "%4d", previousScore);

	//sprintf_s(livesText, 10, "%1d", App->ball->lives);	

	return ret;
}

bool ModuleHud::PostUpdate()
{
	bool ret = true;
	// TODO 3: Blit the text of the score in at the bottom of the screen
	if (drawScore)
	{

		/*app->fonts->BlitText(6, 5, whiteFont, scoreText);
		app->fonts->BlitText(305, 5, redFont, livesText);
		app->fonts->BlitText(106, 5, greenFont, previousScoreText);
		app->fonts->BlitText(206, 5, redFont, highScoreText);*/


		/*if (App->player->destroyed && App->hud->lives == 0 && App->intro->IsEnabled() == false)
		{
			App->fonts->BlitText(78, 185, lightBlueFont, "GAME");
			App->fonts->BlitText(120, 185, lightBlueFont, "OVER");
		}*/
	}


	return ret;
}


bool ModuleHud::CleanUp()
{
	return true;
}
