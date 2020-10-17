#include "Path.h"
#include "Animation.h"

void Path::PushBack(fPoint speed, uint frames, Animation* animation)
{
	steps[totalSteps].animation = animation;
	steps[totalSteps].framesDuration = frames;
	steps[totalSteps].speed = speed;
	
	totalSteps++;
}

bool Path::Finished()
{
	if (currentStep == totalSteps - 1)
	{
		if (currentStepFrame == steps[currentStep].framesDuration)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Path::Update()
{
	currentStepFrame += 1;

	if (currentStepFrame > steps[currentStep].framesDuration)
	{
		if (currentStep < totalSteps - 1)
		{
			currentStep++;
		}
		else if (loop)
		{
			currentStep = 0;
		}
		currentStepFrame = 0;
	}

	relativePosition += steps[currentStep].speed;
}

iPoint Path::GetRelativePosition() const
{
	return iPoint((int)relativePosition.x, (int)relativePosition.y);
}

Animation* Path::GetCurrentAnimation()
{
	return steps[currentStep].animation;
}

void Path::Reset()
{
	currentStepFrame = 0;
	currentStep = 0;
}