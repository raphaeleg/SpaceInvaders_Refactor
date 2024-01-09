#include "Helper.hpp"

static constexpr float PLAYER_BASE_HEIGHT = 70.0f;
static constexpr float PLAYER_RADIUS = 50;
class Player {
private:
	static constexpr float speed = 7;
	static constexpr Vector2 DT_o = { 50,50 };
	int lives = 3;
	float x_pos = SCREEN_WIDTH / 2.0f;
	float timer = 0;
	

public:
	void Render(Texture2D texture) noexcept;
	void Update();
	[[nodiscard]] bool IsDead() const noexcept { return lives <= 0; };
	[[nodiscard]] float GetPosition() const noexcept { return x_pos; };
	[[nodiscard]] int GetLives() const noexcept { return lives; };
	void DecreaseHealth() noexcept { lives--; };
};