#pragma once
#include "raylib.h"
#include <string>

constexpr float SCREEN_HEIGHT = 1080.0f;
constexpr float SCREEN_WIDTH = 1920.0f;
constexpr int SCREEN_HEIGHT_INT = static_cast<int>(SCREEN_HEIGHT);
constexpr int SCREEN_WIDTH_INT = static_cast<int>(SCREEN_WIDTH);
[[nodiscard]] float lineLength(Vector2 A, Vector2 B) noexcept;
[[nodiscard]] bool CheckCollision(Vector2 circleOrigin, float circleRadius, Vector2 projectilePosition) noexcept;
[[nodiscard]] constexpr bool isStrWithinRange(std::string str, int min, int max) noexcept { return str.length() > min && str.length() < max; }