#pragma once
#include "raylib.h"
#include "EntityType.h"

static constexpr float ALIEN_RADIUS = 30;

class Alien
{
private:
    int speed = 2;
    static constexpr Vector2 DT_o = { 50, 50 };

    Vector2 position = { 0, 0 };
    int x = 0;
    int y = 0;
    bool active = true;

public:

    EntityType type = EntityType::ENEMY;

    Alien(int x, int y) noexcept : x(x), y(y) {
        position.x = x;
        position.y = y;
    }
    void Update() noexcept;
    void Render(Texture2D texture) noexcept;
    void Kill() noexcept { active = false; }
    bool IsDead() const noexcept { return !active; }
    bool HasReachedYPosition(int boundary) const noexcept { return position.y > boundary; }
    Vector2 GetPosition() const noexcept { return position; }
};