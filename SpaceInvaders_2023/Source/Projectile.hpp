#pragma once
#include "raylib.h"

struct Projectile
{
private:
    int speed = 15;
    Vector2 position = { 0, 0 };
    static constexpr Vector2 DT_o = { 25,25 };
    bool active = true;
public:
    Vector2 lineStart = { 0, 0 };
    Vector2 lineEnd = { 0, 0 };

    Projectile(Vector2 pos, bool isPlayerProjectile) noexcept;
    void Update() noexcept;
    void Render(Texture2D texture) noexcept;
    void Hit() noexcept { active = false; }
    [[nodiscard]] bool IsPlayerProjectile() const noexcept { return speed < 0; }
    [[nodiscard]] bool IsDead() const noexcept { return !active; }
};