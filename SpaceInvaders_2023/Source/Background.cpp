#include "Background.h"

Background::Background() noexcept {
    for (int i = 0; i < starAmount; i++) {
        const Vector2 tmpPos{ GetRandomValue(-150, GetScreenWidth() + 150), GetRandomValue(0, GetScreenHeight()) };
        const float tmpSize = GetRandomValue(1.0f, 4.0f) / 2.0f;
        Stars.push_back(Star(tmpPos, tmpSize));
    }
}

void Background::Update(float offset) noexcept {
    for (auto& star : Stars) {
        star.Update(offset);
    }
}

void Background::Render() noexcept {
    for (auto& star : Stars) {
        star.Render();
    }
}

void Star::Update(float offset) noexcept {
    position.x = initPosition.x + offset;
    position.y = initPosition.y;
}

void Star::Render() noexcept{
    DrawCircle(static_cast<int>(position.x), static_cast<int>(position.y), size, SKYBLUE);
}