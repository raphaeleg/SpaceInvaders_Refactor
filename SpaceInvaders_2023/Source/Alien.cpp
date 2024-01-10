#include "Alien.hpp"
#include <corecrt_math.h>

void Alien::Update() noexcept {
	position.x += velocity;
	if (IsPositionAtScreenEdge()) {
		GoForward();
	}
}
void Alien::GoForward() noexcept {
	velocity = velocity < 0 ? abs(velocity) : -velocity;
	position.y += 50;
}
void Alien::Render(Texture2D texture) noexcept {
	DrawTexture(texture, static_cast<int>(position.x - (texture.width / 2.0f)), static_cast<int>(position.y - (texture.height / 2.0f)), WHITE);
}