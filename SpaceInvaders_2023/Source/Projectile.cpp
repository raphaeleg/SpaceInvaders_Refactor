#include "Projectile.hpp"

Projectile::Projectile(Vector2 pos, bool isPlayerProjectile) noexcept {
	position = pos;
	if (isPlayerProjectile) { speed = -speed; }
}

void Projectile::Update() noexcept {
	position.y += speed;
	if (position.y < 0 || position.y > 1500) {
		active = false;
	}
}

void Projectile::Render(Texture2D texture) noexcept {
	DrawTexture(texture, position.x, position.y, WHITE);
}