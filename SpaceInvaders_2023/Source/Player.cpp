#include "Player.h"
#include <algorithm>

void Player::UpdateMovement()
{
    int movement = 0;
    if (IsKeyDown(KEY_LEFT)) {
        movement = -1;
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        movement = 1;
    }
    x_pos = std::clamp(x_pos += speed * movement, PLAYER_RADIUS, GetScreenWidth() - PLAYER_RADIUS);
}

void Player::UpdateAnimation() noexcept
{
    timer += GetFrameTime();
    if (timer > animation_speed)
    {
        activeTexture == 2 ? activeTexture = 0 : activeTexture++;
        timer = 0;
    }
}

void Player::Update()
{
    UpdateMovement();
    UpdateAnimation();
}

void Player::Render(Texture2D texture) noexcept
{
    const Rectangle DT_src = { 0,0,static_cast<float>(texture.width), static_cast<float>(texture.height) };
    const Rectangle DT_dst = { x_pos, GetScreenHeight() - PLAYER_BASE_HEIGHT, 100, 100 };
    DrawTexturePro(texture, DT_src, DT_dst, DT_o, 0, WHITE);
}