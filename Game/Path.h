#pragma once

#ifndef __PATH_H__
#define __PATH_H__

#include "P2Point.h"

#define MAX_STEPS 25

typedef unsigned int uint;
typedef unsigned char uchar;

class Animation;

struct Step
{
	uint framesDuration = 1;
	fPoint speed;
	Animation* animation = nullptr;
};

class Path
{

public:

	void PushBack(fPoint speed, uint frame, Animation* animation = nullptr);

	void Update();

	bool Finished();

	iPoint GetRelativePosition() const;

	Animation* GetCurrentAnimation();

	void Reset();

public:

	bool loop = true;

	Step steps[MAX_STEPS];

private:

	uint totalSteps = 0;

	uint currentStepFrame = 0;

	fPoint relativePosition = fPoint(0.0f, 0.0f);

	uint currentStep = 0;

};

#endif __PATH_H__