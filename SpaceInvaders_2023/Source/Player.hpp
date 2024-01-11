#include "Helper.hpp"

static constexpr float PLAYER_BASE_HEIGHT = 70.0f;
static constexpr float PLAYER_RADIUS = 50;
class Player {
private:
	static constexpr float speed = 7;
	int lives = 3;
	Vector2 position = { GetScreenWidthF() / 2.0f, GetScreenHeightF() - PLAYER_BASE_HEIGHT };
	float timer = 0;

public:
	void Render(Texture2D texture) const noexcept;
	void Update();
	[[nodiscard]] bool IsDead() const noexcept { return lives <= 0; };
	[[nodiscard]] float GetPositionX() const noexcept { return position.x; };
	[[nodiscard]] float GetPositionY() const noexcept { return position.y; };
	[[nodiscard]] int GetLives() const noexcept { return lives; };
	void DecreaseHealth() noexcept { lives--; };
};