#include "Background.hpp"
#include "Helper.hpp"
#include "Player.hpp"

Background::Background() noexcept {
	for (int i = 0; i < starAmount; i++) {
		const Vector2 tmpPos{ static_cast<float>(GetRandomValue(-150, GetScreenWidth() + 150)), static_cast<float>(GetRandomValue(0, GetScreenHeight())) };
		const float tmpSize = static_cast<float>(GetRandomValue(1, 4) / 2);
		Stars.push_back(Star(tmpPos, tmpSize));
	}
}

void Background::Update(float _playerPosition) noexcept {
	const Vector2 playerPosition = { _playerPosition, PLAYER_BASE_HEIGHT };
	const Vector2 cornerPosition = { 0, PLAYER_BASE_HEIGHT };
	const float offset = lineLength(playerPosition, cornerPosition) * -1;
	Stars.at(0).Update(offset / 15);
}

void Background::Render() const noexcept {
	for (const auto& star : Stars) {
		star.Render();
	}
}

void Star::Render() const noexcept {
	DrawCircle(static_cast<int>(position.x + offset), static_cast<int>(position.y), size, SKYBLUE);
}