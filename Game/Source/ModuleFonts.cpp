#include "App.h"

#include "Textures.h"
#include "Render.h"
#include "ModuleFonts.h"
#include "Log.h"

#include<string.h>

ModuleFonts::ModuleFonts(bool start_enabled) : Module(start_enabled)
{

}

ModuleFonts::~ModuleFonts()
{

}

// Load new texture from file path
int ModuleFonts::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if(texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = app->tex->Load(texture_path);

	if(tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	//Font& font = fonts[id];
	Font* font = new Font();
	fonts.Add(font);
	font->texture = tex;
	font->rows = rows;

	// TODO 1: Finish storing font data

	// totalLength ---	length of the lookup table
	// table ---------  All characters displayed in the same order as the texture
	// columns -------  Amount of chars per row of the texture
	// char_w --------	Width of each character
	// char_h --------	Height of each character

	/*trcpy_s(list->data->table, MAX_FONT_CHARS, characters);
	list->data->totalLength = strlen(characters);
	list->data->columns = list->data->totalLength / rows;*/

	strcpy_s(font->table, MAX_FONT_CHARS, characters);
	font->totalLength = strlen(characters);
	font->columns = font->totalLength / rows;


	uint tex_w, tex_h;
	app->tex->GetSize(tex, tex_w, tex_h);
	font->char_w = tex_w / font->columns;
	font->char_h = tex_h / font->rows;

	++fontsCount;

	LOG("Successfully loaded BMP font from %s", texture_path);

	ListItem<Font*>* list;
	id = 0;
	list = fonts.At(fonts.Count() - 1);
	id = fonts.Find(list->data);
	
	return id;
}

void ModuleFonts::UnLoad(int font_id)
{
	ListItem<Font*>* list;
	list = fonts.start;
	list = fonts.At(font_id);
	if(font_id >= 0 && font_id < fonts.Count() && list->data->texture != nullptr)
	{
		app->tex->UnLoad(list->data->texture);
		list->data->texture = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

void ModuleFonts::BlitText(int x, int y, int font_id, const char* text) const
{
	Font* data;
	data = fonts.At(font_id)->data;
	if(text == nullptr || font_id < 0 || data->texture == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = data;
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	for(uint i = 0; i < len; ++i)
	{
		// TODO 2: Find the character in the table and its position in the texture, then Blit
		uint charIndex = 0;

		// Find the location of the current character in the lookup table
		for (uint j = 0; j < font->totalLength; ++j)
		{
			if (font->table[j] == text[i])
			{
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		app->render->DrawTexture(font->texture, x, y, &spriteRect, 0.0f, false);

		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}
