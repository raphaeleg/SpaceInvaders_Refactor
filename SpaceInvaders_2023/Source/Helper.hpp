#pragma once
#include <raylib.h>

constexpr float SCREEN_HEIGHT = 1080.0f;
constexpr float SCREEN_WIDTH = 1920.0f;
constexpr int SCREEN_HEIGHT_INT = static_cast<int>(SCREEN_HEIGHT);
constexpr int SCREEN_WIDTH_INT = static_cast<int>(SCREEN_WIDTH);
float lineLength(Vector2 A, Vector2 B) noexcept;
bool CheckCollision(Vector2 C, float circleRadius, Vector2 A, Vector2 B) noexcept;