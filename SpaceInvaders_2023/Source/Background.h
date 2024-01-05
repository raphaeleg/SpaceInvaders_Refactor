#pragma once
#include <raylib.h>
#include <vector>

class Star
{
private:
    Vector2 initPosition;
    Vector2 position;
    float size;
public:
    Star(Vector2 pos, float sz) noexcept : initPosition(pos), position(pos), size(sz) {};
    void Update(float offset) noexcept;
    void Render() noexcept;
};

class Background
{
private:
    static constexpr int starAmount = 600;
    std::vector<Star> Stars;

public:
    Background() noexcept;
    void Update(float offset) noexcept;
    void Render() noexcept;
};