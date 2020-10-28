#include "App.h"

#include "Defs.h"
#include "Log.h"

// NOTE: SDL redefines main function
#include "SDL/include/SDL.h"

// NOTE: Library linkage is configured in Linker Options
//#pragma comment(lib, "../Game/Source/External/SDL/libx86/SDL2.lib")
//#pragma comment(lib, "../Game/Source/External/SDL/libx86/SDL2main.lib")

#include <stdlib.h>

enum MainState
{
	CREATE = 1,
	AWAKE,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

App* app = NULL;

// How many frames time values to keep
// The higher the value the smoother the result is...
// Don't make it 0 or less :)
#define FRAME_VALUES 10

// An array to store frame times:
Uint32 frametimes[FRAME_VALUES];

// Last calculated SDL_GetTicks
Uint32 frametimelast;

// total frames rendered
Uint32 framecount;

// the value you want
float framespersecond;

// This function gets called once on startup.
void fpsinit() {

	// Set all frame times to 0ms.
	memset(frametimes, 0, sizeof(frametimes));
	framecount = 0;
	framespersecond = 0;
	frametimelast = SDL_GetTicks();

}

void fpsthink() {

	Uint32 frametimesindex;
	Uint32 getticks;
	Uint32 count;
	Uint32 i;

	// frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
	// This value rotates back to 0 after it hits FRAME_VALUES.
	frametimesindex = framecount % FRAME_VALUES;

	// store the current time
	getticks = SDL_GetTicks();

	// save the frame time value
	frametimes[frametimesindex] = getticks - frametimelast;

	// save the last frame time for the next fpsthink
	frametimelast = getticks;

	// increment the frame count
	framecount++;

	// Work out the current framerate

	// The code below could be moved into another function if you don't need the value every frame.

	// I've included a test to see if the whole array has been written to or not. This will stop
	// strange values on the first few (FRAME_VALUES) frames.
	if (framecount < FRAME_VALUES) {

		count = framecount;

	}
	else {

		count = FRAME_VALUES;

	}

	// add up all the values and divide to get the average frame time.
	framespersecond = 0;
	for (i = 0; i < count; i++) {

		framespersecond += frametimes[i];

	}

	framespersecond /= count;

	// now to make it an actual frames per second value...
	framespersecond = 1000.f / framespersecond;

}


int main(int argc, char* args[])
{
	LOG("Engine starting ...");

	MainState state = CREATE;
	int result = EXIT_FAILURE;

	fpsinit();

	while(state != EXIT)
	{
		Uint32 start = SDL_GetTicks();
		switch(state)
		{

			// Allocate the engine --------------------------------------------
			case CREATE:
			LOG("CREATION PHASE ===============================");

			app = new App(argc, args);

			if(app != NULL)
				state = AWAKE;
			else
				state = FAIL;

			break;

			// Awake all modules -----------------------------------------------
			case AWAKE:
			LOG("AWAKE PHASE ===============================");
			if(app->Awake() == true)
				state = START;
			else
			{
				LOG("ERROR: Awake failed");
				state = FAIL;
			}

			break;

			// Call all modules before first frame  ----------------------------
			case START:
			LOG("START PHASE ===============================");
			if(app->Start() == true)
			{
				state = LOOP;
				LOG("UPDATE PHASE ===============================");
			}
			else
			{
				state = FAIL;
				LOG("ERROR: Start failed");
			}
			break;

			// Loop all modules until we are asked to leave ---------------------
			case LOOP:
			if(app->Update() == false)
				state = CLEAN;
			break;

			// Cleanup allocated memory -----------------------------------------
			case CLEAN:
			LOG("CLEANUP PHASE ===============================");
			if(app->CleanUp() == true)
			{
				RELEASE(app);
				result = EXIT_SUCCESS;
				state = EXIT;
			}
			else
				state = FAIL;

			break;

			// Exit with errors and shame ---------------------------------------
			case FAIL:
			LOG("Exiting with errors :(");
			result = EXIT_FAILURE;
			state = EXIT;
			break;
		}
		Uint32 end = SDL_GetTicks();
		float secondsElapsed = (end - start) / 1000.0f;
		fpsthink();
		printf("%f\n", framespersecond);

		if ((1.0f / 60.0f) - secondsElapsed > 0.0f)
		{
			SDL_Delay(secondsElapsed);
		}
	}

	LOG("... Bye! :)\n");

	// Dump memory leaks
	return result;
}