#include "Wall.hpp"

void Wall::Render(Texture2D texture) const noexcept {
	const int renderPositionX = static_cast<int>(GetPosition().x - (texture.width / 2.0f));
	const int renderPositionY = static_cast<int>(GetPosition().y - (texture.height / 2.0f));
	DrawTexture(texture, renderPositionX, renderPositionY, WHITE);
	DrawText(TextFormat("%i", health), static_cast<int>(healthTextPosition.x), static_cast<int>(healthTextPosition.y), fontSize, RED);
}