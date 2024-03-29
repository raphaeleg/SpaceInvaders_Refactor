#include "Projectile.hpp"

Projectile::Projectile(Vector2 pos, bool isPlayerProjectile) noexcept {
	position = pos;
	if (isPlayerProjectile) { speed = -speed; }
}

void Projectile::Render(Texture2D texture) const noexcept {
	const int renderPositionX = static_cast<int>(GetPosition().x - (texture.width / 2.0f));
	const int renderPositionY = static_cast<int>(GetPosition().y - (texture.height / 2.0f));
	DrawTexture(texture, renderPositionX, renderPositionY, WHITE);
}