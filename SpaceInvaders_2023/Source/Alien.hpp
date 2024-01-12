#pragma once
#include "raylib.h"
#include "Helper.hpp"

static constexpr float ALIEN_RADIUS = 30;
class Alien {
private:
    int velocity = 2;
    Vector2 position = { 0, 0 };

    void GoForward() noexcept;
    [[nodiscard]] bool IsPositionAtScreenEdge() const noexcept { return GetPosition().x < 0 || GetPosition().x > GetScreenWidthF(); }
public:
    Alien(const Vector2& pos) noexcept : position(pos) {};
    void Update() noexcept;
    void Render(Texture2D texture) const noexcept;
    [[nodiscard]] Vector2 GetPosition() const noexcept { return position; }
    [[nodiscard]] bool HasReachedYPosition(float boundary) const noexcept { return GetPosition().y > boundary; }
};