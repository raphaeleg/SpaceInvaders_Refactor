#pragma once
#include "raylib.h"

static constexpr int PROJECTILE_LENGTH = 15;
class Projectile {
private:
	int speed = 15;
	Vector2 position = { 0, 0 };
public:
	Projectile(Vector2 pos, bool isPlayerProjectile) noexcept;
	void Update() noexcept;
	void Render(Texture2D texture) const noexcept;
	[[nodiscard]] Vector2 GetPosition() const noexcept { return position; }
	[[nodiscard]] bool isOutOfScreen() const noexcept { return GetPosition().y < 0 || GetPosition().y > GetScreenHeightF(); }
};