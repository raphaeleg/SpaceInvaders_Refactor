#pragma once
#include "raylib.h"
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
    std::vector<Star> Stars;

public:
    Background() noexcept;
    void Render(float _playerPosition) const noexcept;
};