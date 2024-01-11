#pragma once
#include "raylib.h"
#include "Helper.hpp"

static constexpr float ALIEN_RADIUS = 30;
class Alien {
private:
    int velocity = 2;
    Vector2 position = { 0, 0 };

    void GoForward() noexcept;
    [[nodiscard]] bool IsPositionAtScreenEdge() const noexcept { return position.x < 0 || position.x > GetScreenWidthF(); }
public:
    Alien(const Vector2& pos) noexcept : position(pos) {};
    void Update() noexcept;
    void Render(Texture2D texture) const noexcept;
    [[nodiscard]] bool HasReachedYPosition(int boundary) const noexcept { return position.y > boundary; }
    [[nodiscard]] Vector2 GetPosition() const noexcept { return position; }
};