#include "ModuleFadeToBlack.h"

#include "App.h"
#include "Render.h"
#include "Window.h"

#include "SDL/include/SDL_render.h"
#include "Log.h"

ModuleFadeToBlack::ModuleFadeToBlack(bool startEnabled) : Module()
{
	uint width, height;
	app->win->GetWindowSize(width, height);
	screenRect = {0, 0, (int)(width * app->win->GetScale()), (int)(height * app->win->GetScale()) };
	name = "fade";
}

ModuleFadeToBlack::ModuleFadeToBlack() :Module(true)
{
	name = "fade";
	
}

bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	uint width, height;
	app->win->GetWindowSize(width, height);
	screenRect = { 0, 0, (int)(width * app->win->GetScale()), (int)(height * app->win->GetScale()) };	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool ModuleFadeToBlack::Update(float dt)
{
	bool ret = true;
	// Exit this function if we are not performing a fade
	if (currentStep == FadeStep::NONE) return ret;

	if (currentStep == FadeStep::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			// TODO 1: Enable / disable the modules received when FadeToBlacks() gets called
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = FadeStep::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = FadeStep::NONE;
		}
	}

	return ret;
}

bool ModuleFadeToBlack::PostUpdate()
{
	bool ret = true;
	// Exit this function if we are not performing a fade
	if (currentStep == FadeStep::NONE) return ret;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return ret;
}

bool ModuleFadeToBlack::FadeToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = true;

	// If we are already in a fade process, ignore this call
	if(currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		// TODO 1: We need to keep track of the modules received in FadeToBlack(...)
		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}

	return ret;
}