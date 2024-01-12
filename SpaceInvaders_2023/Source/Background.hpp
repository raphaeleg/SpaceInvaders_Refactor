#pragma once
#include "raylib.h"
#include <vector>

class Star {
private:
    Vector2 position;    
    float size;
public:
    Star(const Vector2& pos, const float sz) noexcept : position(pos), size(sz) {};
    void Render(float offset) const noexcept;
};

class Background {
private:
    static constexpr int starAmount = 600;
    std::vector<Star> Stars;
public:
    Background() noexcept;
    void Render(const Vector2& playerPosition, const Vector2& cornerPosition) const noexcept;
};