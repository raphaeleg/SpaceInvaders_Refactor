#include "Alien.hpp"
#include "Helper.hpp"
#include <corecrt_math.h>

Alien::Alien(float x, float y) noexcept {
	position.x = x;
	position.y = y;
}

void Alien::Update() noexcept {
	position.x += speed;
	if (position.x <= 0 || position.x >= SCREEN_WIDTH)
	{
		speed = speed < 0 ? abs(speed) : -speed;
		position.y += 50;
	}
}

void Alien::Render(Texture2D texture) const noexcept {
	const int renderPositionX = static_cast<int>(GetPosition().x - (texture.width / 2.0f));
	const int renderPositionY = static_cast<int>(GetPosition().y - (texture.height / 2.0f));
	DrawTexture(texture, renderPositionX, renderPositionY, WHITE);
}