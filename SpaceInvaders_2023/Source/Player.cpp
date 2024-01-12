#include "Player.hpp"
#include <algorithm>

void Player::Update() {
	int movement = 0;
	if (IsKeyDown(KEY_LEFT)) {
		movement = -1;
	}
	else if (IsKeyDown(KEY_RIGHT)) {
		movement = 1;
	}
	position.x += speed * movement;
	position.x = std::clamp(GetPositionX(), PLAYER_RADIUS, GetScreenWidthF() - PLAYER_RADIUS);
}

void Player::Render(Texture2D texture) const noexcept {
	const int renderPositionX = static_cast<int>(position.x - (texture.width / 2.0f));
	const int renderPositionY = static_cast<int>(position.y - (texture.height / 2.0f));
	DrawTexture(texture, renderPositionX, renderPositionY, WHITE);
}