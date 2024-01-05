#include "Player.h"

void Player::Initialize()
{
	float window_width = (float)GetScreenWidth();
	x_pos = window_width / 2;
}

void Player::Update()
{
	direction = 0;
	if (IsKeyDown(KEY_LEFT))
	{
		direction--;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		direction++;
	}

	x_pos += speed * direction;

	if (x_pos < 0 + radius)
	{
		x_pos = 0 + radius;
	}
	else if (x_pos > GetScreenWidth() - radius)
	{
		x_pos = GetScreenWidth() - radius;
	}

	timer += GetFrameTime();

	if (timer > 0.4 && activeTexture == 2)
	{
		activeTexture = 0;
		timer = 0;
	}
	else if (timer > 0.4)
	{
		activeTexture++;
		timer = 0;
	}
}

void Player::Render(Texture2D texture)
{
	float window_height = GetScreenHeight();
	DrawTexturePro(texture, { 0,0,352,352 }, { x_pos, window_height - player_base_height,100,100 }, { 50,50 }, 0, WHITE);
}