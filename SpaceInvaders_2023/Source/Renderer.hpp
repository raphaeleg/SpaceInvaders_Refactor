#include "Helper.hpp"
#include "raylib.h"
#include <string>
// TODO: all rendering should be const
static constexpr int textPosX_left = 50;
static constexpr int fontSize_M = 40;
class Renderer {
private:
	static constexpr Rectangle textBox = { 600, 500, 225, 50 };
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

	[[nodiscard]] bool textBoxX() const noexcept { return static_cast<int>(textBox.x); }
	[[nodiscard]] bool textBoxY() const noexcept { return static_cast<int>(textBox.y); }
	[[nodiscard]] bool textBoxWidth() const noexcept { return static_cast<int>(textBox.width); }
	[[nodiscard]] bool textBoxHeight() const noexcept { return static_cast<int>(textBox.height); }

public:
	[[nodiscard]] int GetPlayerActiveTexture() const noexcept { return playerActiveTexture; }
	[[nodiscard]] bool mouseOnText() const noexcept { return CheckCollisionPointRec(GetMousePosition(), textBox); }
	
	void UpdatePlayerAnimation() noexcept;
	void StartScreen() noexcept;
	void GameplayText(int score, int lives) noexcept;
	void DefaultEndScreen() noexcept;
	void HighscoreScreen(std::string name) noexcept;
};