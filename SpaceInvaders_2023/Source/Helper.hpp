#pragma once
#include "raylib.h"
#include <string>

inline float GetScreenHeightF() noexcept { return static_cast<float>(GetScreenHeight()); }
inline float GetScreenWidthF() noexcept { return static_cast<float>(GetScreenWidth()); }
float lineLength(Vector2 A, Vector2 B) noexcept;
bool CheckCollision(Vector2 C, float circleRadius, Vector2 projectilePosition) noexcept;