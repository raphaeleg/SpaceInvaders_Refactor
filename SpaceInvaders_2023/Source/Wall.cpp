#include "Wall.h"

void Wall::Render(Texture2D texture)
{
	DrawTexturePro(texture, { 0,0,704,704 }, { position.x,position.y,200,200 }, { 100,100 }, 0, WHITE);
	DrawText(TextFormat("%i", health), position.x - 21, position.y + 10, 40, RED);
}

void Wall::Update()
{
	if (health < 1)
	{
		active = false;
	}
}