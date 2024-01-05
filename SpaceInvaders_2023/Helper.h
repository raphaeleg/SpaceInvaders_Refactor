#pragma once
#include <raylib.h>

float lineLength(Vector2 A, Vector2 B) noexcept;
bool CheckCollision(Vector2 circleOrigin, float circleRadius, Vector2 A, Vector2 B) noexcept;