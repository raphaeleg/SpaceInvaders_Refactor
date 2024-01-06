#include "Background.hpp"

Background::Background() noexcept {
    for (int i = 0; i < starAmount; i++) {
        const Vector2 tmpPos{ static_cast<float>(GetRandomValue(-150, GetScreenWidth() + 150)), static_cast<float>(GetRandomValue(0, GetScreenHeight())) };
        const float tmpSize = static_cast<float>(GetRandomValue(1, 4) / 2);
        Stars.push_back(Star(tmpPos, tmpSize));
    }
}

void Background::Update(float offset) noexcept {
    Stars.at(0).Update(offset);
}

void Background::Render() noexcept {
    for (auto& star : Stars) {
        star.Render();
    }
}

void Star::Render() noexcept{
    DrawCircle(static_cast<int>(position.x + offset), static_cast<int>(position.y), size, SKYBLUE);
}