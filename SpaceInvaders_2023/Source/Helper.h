#pragma once
#include <raylib.h>

float lineLength(Vector2 A, Vector2 B) noexcept;
bool CheckCollision(Vector2 C, float circleRadius, Vector2 A, Vector2 B) noexcept;