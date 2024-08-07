#include "Helper.hpp"

static constexpr float PLAYER_BASE_HEIGHT = 70.0f;
static constexpr float PLAYER_RADIUS = 50;
class Player {
private:
	static constexpr float speed = 7;
	int lives = 3;
	Vector2 position = { GetScreenWidthF() / 2.0f, GetScreenHeightF() - PLAYER_BASE_HEIGHT };

public:
	void Update();
	void Render(Texture2D texture) const noexcept;
	[[nodiscard]] bool IsDead() const noexcept { return lives <= 0; };
	[[nodiscard]] Vector2 GetPosition() const noexcept { return position; };
	[[nodiscard]] float GetPositionX() const noexcept { return GetPosition().x; };
	[[nodiscard]] float GetPositionY() const noexcept { return GetPosition().y; };
	[[nodiscard]] int GetLives() const noexcept { return lives; };
	void DecreaseHealth() noexcept { lives--; };
};