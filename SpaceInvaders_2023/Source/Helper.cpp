#include "Helper.hpp"
#include "Projectile.hpp"
#include "raylib.h"
#include <corecrt_math.h>

float lineLength(Vector2 A, Vector2 B) noexcept
{
	return sqrtf(static_cast<float>(pow(B.x - A.x, 2) + pow(B.y - A.y, 2)));
}

static bool pointInCircle(Vector2 circlePos, float radius, Vector2 point) noexcept
{
	return lineLength(circlePos, point) < radius;
}

bool CheckCollision(Vector2 circleOrigin, float circleRadius, Vector2 projectilePosition) noexcept
{
	const Vector2 A = { projectilePosition.x, projectilePosition.y - PROJECTILE_LENGTH };
	const Vector2 B = { projectilePosition.x, projectilePosition.y + PROJECTILE_LENGTH };

	if (pointInCircle(circleOrigin, circleRadius, A) || pointInCircle(circleOrigin, circleRadius, B)) {
		return true;
	}

	constexpr float buffer = 0.1f;
	const float length = lineLength(A, B);
	const float dotP = static_cast<float>((((circleOrigin.x - A.x) * (B.x - A.x)) + ((circleOrigin.y - A.y) * (B.y - A.y))) / pow(length, 2));

	const float closestX = A.x + (dotP * (B.x - A.x));
	const float closestY = A.y + (dotP * (B.y - A.y));

	const float closeToStart = lineLength(A, { closestX, closestY });
	const float closeToEnd = lineLength(B, { closestX, closestY });

	const float closestLength = closeToStart + closeToEnd;

	if (closestLength == length + buffer || closestLength == length - buffer) {
		return closeToStart < circleRadius;
	}
	return false;
}