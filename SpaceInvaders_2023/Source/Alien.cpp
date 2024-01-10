#include "Alien.hpp"
#include "Helper.hpp"
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
void Alien::Render(Texture2D texture) const noexcept {
	const int renderPositionX = static_cast<int>(GetPosition().x - (texture.width / 2.0f));
	const int renderPositionY = static_cast<int>(GetPosition().y - (texture.height / 2.0f));
	DrawTexture(texture, renderPositionX, renderPositionY, WHITE);
}