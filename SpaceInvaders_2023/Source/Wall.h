#pragma once
#include "raylib.h"

static constexpr int WALL_RADIUS = 60;

class Wall
{
private:
    Vector2 position;
    int health = 50;

    Rectangle DT_dst = { position.x, position.y, 200, 200 };
    static constexpr Vector2 DT_o = { 100,100 };
    int DT_posX = position.x - 21;
    int DT_posY = position.y + 10;
    static constexpr int DT_fontSize = 40;

public:
    Wall(float posX, float posY) noexcept : position{ posX, posY } {};
    void Render(Texture2D texture) noexcept;
    void DecreaseHealth() noexcept { health -= 1; };
    Vector2 GetPosition() const noexcept { return position; };
    bool IsNotActive() const noexcept { return health < 1; };
};