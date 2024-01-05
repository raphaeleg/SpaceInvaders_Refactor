#pragma once
#include "raylib.h"

struct Wall
{
public:
    Vector2 position;
    Rectangle rec;
    bool active;
    Color color;
    int health = 50;
    int radius = 60;

    void Render(Texture2D texture);
    void Update();
};