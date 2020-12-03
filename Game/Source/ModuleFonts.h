#ifndef __MODULE_FONTS_H__
#define __MODULE_FONTS_H__

#include "Module.h"
#include "SDL/include/SDL_pixels.h"
#include "List.h"

#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font
{
	// Lookup table. All characters displayed in the same order as the itemsTexture
	char table[MAX_FONT_CHARS];

	// The font itemsTexture
	SDL_Texture* itemsTexture = nullptr;

	// Font setup data
	uint totalLength;
	uint rows, columns;
	uint char_w, char_h;
};

class ModuleFonts : public Module
{
public:

	// Constructor
	ModuleFonts(bool isEnabled = true);

	// Destructor
	~ModuleFonts();

	// Loads a font file from a itemsTexture
	// Returns a font index from the fonts array
	// Param texturePath	- The path to the itemsTexture file
	// Param characters		- The lookup table. All characters displayed in the same order as the itemsTexture
	// Param rows			- The amount of character rows in the itemsTexture
	int Load(const char* texturePath, const char* characters, uint rows = 1);

	// Removes a font by its index
	// Unloads the itemsTexture and removes it from the fonts array
	void UnLoad(int fontIndex);

	// Create a surface from text
	void BlitText(int x, int y, int fontIndex, const char* text) const;

	inline uint GetFontsCount() const { return fonts.Count(); };
	List<Font*>fonts;

private:
	// An array to keep track and store all loaded fonts
	//Font fonts[MAX_FONTS];
	// The amount of fonts loaded into the array
	uint fontsCount = 0;
};


#endif // __ModuleFonts_H__