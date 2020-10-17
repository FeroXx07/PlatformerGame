#pragma once

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#ifdef _DEBUG
#pragma comment( lib, "Game/Source/External/SDL/include/SDL_rect.h")
#else
#pragma comment( lib, "Game/Source/External/SDL/include/SDL_rect.h")
#endif
#define MAX_FRAMES 100

class Animation
{
public:

	bool loop = true;
	bool flow = false;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:

	float currentFrame = 0.0f;
	int lastFrame = 0;
	int loops = 0;
	enum flow
	{
		FORDWARDS,
		BACKWARDS
	} direction = FORDWARDS;

public:

	Animation() {}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), lastFrame(anim.lastFrame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[lastFrame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		switch (direction)
		{
		case flow::FORDWARDS:
		{
			currentFrame += speed;
			if (currentFrame >= lastFrame)
			{
				currentFrame = (loop || flow) ? 0.0f : lastFrame - 1;
				direction = flow ? flow::BACKWARDS : flow::FORDWARDS;
				loops++;
			}
		}
		break;

		case flow::BACKWARDS:
		{
			currentFrame -= speed;
			if (currentFrame <= 0.0f)
			{
				currentFrame = 0.0f;
				direction = flow::FORDWARDS;
				loops++;
			}
		}
		break;
		}
			
		return frames[(int)currentFrame];
	}

	SDL_Rect& GetFrame(int frameNumber)
	{
		return frames[frameNumber];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Update()
	{
		currentFrame += speed;

		if (currentFrame >= lastFrame)
		{
			currentFrame = (loop) ? 0.0f : lastFrame - 1;
			++loops;
		}
	}

	void Reset()
	{
		loops = 0;
		currentFrame = 0.0f;
	}

};


#endif __ANIMATION_H__