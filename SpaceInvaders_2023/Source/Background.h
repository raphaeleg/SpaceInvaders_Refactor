#pragma once
#include <raylib.h>
#include <vector>

struct Star
{
    Vector2 initPosition = { 0, 0 };
    Vector2 position = { 0, 0 };
    Color color = GRAY;
    float size = 0;
    void Update(float starOffset);
    void Render();
};

struct Background
{
    std::vector<Star> Stars;

    void Initialize(int starAmount);
    void Update(float offset);
    void Render();
};