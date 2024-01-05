#pragma once
#include "raylib.h"
#include "EntityType.h"

struct Player
{
public:
    float x_pos = 0;
    float speed = 7;
    float player_base_height = 70.0f;
    float radius = 50;
    int lives = 3;
    int direction = 0;
    int activeTexture = 0;
    float timer = 0;

    EntityType type = EntityType::PLAYER;

    void Initialize();
    void Render(Texture2D texture);
    void Update();
};