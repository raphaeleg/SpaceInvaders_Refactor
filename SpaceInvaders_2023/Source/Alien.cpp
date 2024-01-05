#include "Alien.h"

void Alien::Update()
{
	int window_width = GetScreenWidth();

	if (moveRight)
	{
		position.x += speed;

		if (position.x >= GetScreenWidth())
		{
			moveRight = false;
			position.y += 50;
		}
	}
	else
	{
		position.x -= speed;

		if (position.x <= 0)
		{
			moveRight = true;
			position.y += 50;
		}
	}
}

void Alien::Render(Texture2D texture)
{
	DrawTexturePro(texture, { 0,0,352,352 }, { position.x,position.y,100,100 }, { 50,50 }, 0, WHITE);
}