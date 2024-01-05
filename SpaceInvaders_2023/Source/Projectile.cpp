#include "Projectile.h"

Projectile::Projectile(Vector2 pos, bool isPlayerProjectile) noexcept {
    position = pos;
    isPlayerProjectile ? speed = -speed : NULL;
}

void Projectile::Update() noexcept
{
    position.y += speed;

    lineStart.y = position.y - 15;
    lineEnd.y = position.y + 15;

    lineStart.x = position.x;
    lineEnd.x = position.x;

    if (position.y < 0 || position.y > 1500)
    {
        active = false;
    }
}

void Projectile::Render(Texture2D texture) noexcept
{
    const Rectangle DT_src = { 0,0,static_cast<float>(texture.width), static_cast<float>(texture.height) };
    const Rectangle DT_dst = { position.x,position.y,50,50 };
    DrawTexturePro(texture, DT_src, DT_dst, DT_o, 0, WHITE);
}