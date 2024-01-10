#include "Wall.hpp"

void Wall::Render(Texture2D texture) noexcept {
	DrawTexture(texture, static_cast<int>(position.x - (texture.width / 2.0f)), static_cast<int>(position.y - (texture.height / 2.0f)), WHITE);
	DrawText(TextFormat("%i", health), static_cast<int>(healthText.x), static_cast<int>(healthText.y), fontSize, RED);
}