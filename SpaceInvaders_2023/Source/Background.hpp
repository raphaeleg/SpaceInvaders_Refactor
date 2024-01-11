#pragma once
#include "raylib.h"
#include "Player.hpp"
#include "Helper.hpp"
#include <vector>

class Star {
private:
    Vector2 position;    
    float size;
public:
    Star(Vector2 pos, float sz) noexcept : position(pos), size(sz) {};
    void Render(float offset) const noexcept;
};

class Background {
private:
    static constexpr int starAmount = 600;
    static constexpr Vector2 cornerPosition = { 0, PLAYER_BASE_HEIGHT };
    std::vector<Star> Stars;
public:
    Background() noexcept;
    void Render(Vector2 playerPosition) const noexcept;
};