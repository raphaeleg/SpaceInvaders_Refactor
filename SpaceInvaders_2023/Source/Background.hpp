#pragma once
#include "raylib.h"
#include <vector>

class Star {
private:
    Vector2 position;    
    float size;
    inline static float offset;

public:
    Star(Vector2 pos, float sz) noexcept : position(pos), size(sz) {};
    void Update(float _offset) noexcept { offset = _offset; }
    void Render() const noexcept;
};

class Background{
private:
    static constexpr int starAmount = 600;
    std::vector<Star> Stars;

public:
    Background() noexcept;
    void Update(float playerPosition) noexcept;
    void Render() const noexcept;
};