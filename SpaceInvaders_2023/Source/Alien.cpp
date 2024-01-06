#include "Alien.hpp"
#include "Helper.hpp"
#include <corecrt_math.h>

Alien::Alien(float x, float y) noexcept {
    position.x = x;
    position.y = y;
}

void Alien::Update() noexcept
{
    position.x += speed;
    if (position.x <= 0 || position.x >= SCREEN_WIDTH)
    {
        speed = speed < 0 ? abs(speed) : -speed;
        position.y += 50;
    }
}

void Alien::Render(Texture2D texture) noexcept
{
    const Rectangle DT_src = { 0,0,static_cast<float>(texture.width), static_cast<float>(texture.height) };
    const Rectangle DT_dst = { position.x, position.y, 100, 100 };
    DrawTexturePro(texture, DT_src, DT_dst, DT_o, 0, WHITE);
}