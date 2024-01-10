#pragma once
#include "raylib.h"

static constexpr int PROJECTILE_LENGTH = 15;
class Projectile {
private:
	int speed = 15;
	Vector2 position = { 0, 0 };
	bool active = true;
public:
	Projectile(Vector2 pos, bool isPlayerProjectile) noexcept;
	void Update() noexcept;
	void Render(Texture2D texture) const noexcept;
	void Hit() noexcept { active = false; }
	[[nodiscard]] bool IsPlayerProjectile() const noexcept { return speed < 0; }
	[[nodiscard]] bool IsDead() const noexcept { return !active; }
	[[nodiscard]] Vector2 GetPosition() const noexcept { return position; }
};