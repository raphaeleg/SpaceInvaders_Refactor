#include "Leaderboard.hpp"
#include "Helper.hpp"
#include "raylib.h"
#include <string>

static constexpr int textPosX_left = 50;
static constexpr int fontSize_M = 40;
class Renderer {
private:
	static constexpr auto textBoxX = 600;
	static constexpr auto textBoxY = 500;
	static constexpr auto textBoxWidth = 225;
	static constexpr auto textBoxHeight = 50;
	static constexpr Rectangle textBox = { textBoxX, textBoxY, textBoxWidth, textBoxHeight };
	static constexpr int fontSize_XL = 160;
	static constexpr int fontSize_L = 60;
	static constexpr int fontSize_S = 20;
	static constexpr int textPosX = 600;
	int framesCounter = 0;
	static constexpr float animationSpeed = 0.4f;
	int playerActiveTexture = 0;
	float playerAnimationTimer = 0;

	void HighlightTextbox() noexcept;
	[[nodiscard]] bool showUnderscoreInTextbox() const noexcept { return ((framesCounter / 20) % 2) == 0; }
	
public:
	void UpdatePlayerAnimation() noexcept;
	void StartScreen() noexcept;
	void DefaultEndScreen() noexcept;
	void HighscoreScreen(std::string name) noexcept;
	[[nodiscard]] int GetPlayerActiveTexture() const noexcept { return playerActiveTexture; }
	[[nodiscard]] bool mouseOnText() noexcept { return CheckCollisionPointRec(GetMousePosition(), textBox); }
};