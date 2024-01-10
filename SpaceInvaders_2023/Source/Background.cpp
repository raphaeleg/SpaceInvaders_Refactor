#include "Background.hpp"
#include "Player.hpp"

Background::Background() noexcept {
	for (int i = 0; i < starAmount; i++) {
		const Vector2 tmpPos{ static_cast<float>(GetRandomValue(0, GetScreenWidth() + 150)), static_cast<float>(GetRandomValue(0, GetScreenHeight())) };
		const float tmpSize = static_cast<float>(GetRandomValue(1, 4) / 2);
		Stars.emplace_back(Star(tmpPos, tmpSize));
	}
}
void Background::Render(float _playerPosition) const noexcept {
	const Vector2 playerPosition = { _playerPosition, PLAYER_BASE_HEIGHT };
	const Vector2 cornerPosition = { 0, PLAYER_BASE_HEIGHT };
	const auto offset = lineLength(playerPosition, cornerPosition) * -1;

	for (const auto& star : Stars) {
		star.Render(offset / 15);
	}
}
void Star::Render(float offset) const noexcept {
	DrawCircle(static_cast<int>(position.x + offset), static_cast<int>(position.y), size, SKYBLUE);
}