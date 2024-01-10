#pragma once
#include "raylib.h"

static constexpr int WALL_RADIUS = 60;
class Wall {
private:
	Vector2 position;
	int health = 50;
	Vector2 healthText = { position.x - 21, position.y + 10 };
	static constexpr int fontSize = 40;

public:
	Wall(float posX, float posY) noexcept : position{ posX, posY } {};
	void Render(Texture2D texture) noexcept;
	void DecreaseHealth() noexcept { health -= 1; };
	[[nodiscard]] Vector2 GetPosition() const noexcept { return position; };
	[[nodiscard]] bool IsDead() const noexcept { return health < 1; };
};