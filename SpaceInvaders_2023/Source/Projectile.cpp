#include "Projectile.hpp"

Projectile::Projectile(Vector2 pos, bool isPlayerProjectile) noexcept {
	position = pos;
	if (isPlayerProjectile) { speed = -speed; }
}

void Projectile::Update() noexcept {
	position.y += speed;
}

void Projectile::Render(Texture2D texture) noexcept {
	DrawTexture(texture, static_cast<int>(position.x - (texture.width / 2.0f)), static_cast<int>(position.y - (texture.height / 2.0f)), WHITE);
}