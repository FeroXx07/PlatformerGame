#include "Entity.h"

class Font;

class Hud : public Entity
{
public:
	Hud();
	~Hud();

	bool Update(float dt) override;
	bool Draw() override;
	bool HandleInput(float dt) override;

	SDL_Texture* itemsTexture = nullptr;
	SDL_Texture* aimTexture = nullptr;
	SDL_Rect spriteStar;
	SDL_Rect spriteHealth;
	SDL_Rect spriteAim;
	SDL_Rect spriteLifes;
	SDL_Rect spriteHearts;
	
	int lifesFont = -1;
	int starsFont = -1;
	
	char livesText[10] = { "\0"};
	char starText[10] = { "\0" };
	char healthText[10] = { "\0" };
	
	int drawScore = 0;

	Font* font;
};