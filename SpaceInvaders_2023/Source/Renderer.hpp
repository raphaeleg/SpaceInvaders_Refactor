#include "Leaderboard.hpp"
#include "Helper.hpp"
#include "raylib.h"
#include <string>

namespace Renderer {
	std::string draftHighscoreName;
	static constexpr auto textBoxX = 600;
	static constexpr auto textBoxY = 500;
	static constexpr auto textBoxWidth = 225;
	static constexpr auto textBoxHeight = 50;
	static constexpr Rectangle textBox = { textBoxX, textBoxY, textBoxWidth, textBoxHeight };
	static constexpr int fontSize_XL = 160;
	static constexpr int fontSize_L = 60;
	static constexpr int fontSize_M = 40;
	static constexpr int fontSize_S = 20;
	static constexpr int textPosX_left = 50;
	static constexpr int textPosX = 600;
	int framesCounter = 0;
	static constexpr float animationSpeed = 0.4f;
	int playerActiveTexture = 0;
	float playerAnimationTimer = 0;

	void StartScreen() noexcept {
		DrawText("SPACE INVADERS", 200, 100, fontSize_XL, YELLOW);
		DrawText("PRESS SPACE TO BEGIN", 200, 350, fontSize_M, YELLOW);
	}

	[[nodiscard]] bool mouseOnText() noexcept { return CheckCollisionPointRec(GetMousePosition(), textBox); }
	[[nodiscard]] bool showUnderscoreInTextbox() noexcept { return ((framesCounter / 20) % 2) == 0; }
	void HighlightTextbox() noexcept {
		if (mouseOnText()) {
			SetMouseCursor(MOUSE_CURSOR_IBEAM);
			framesCounter++;
			DrawRectangleLines(textBoxX, textBoxY, textBoxWidth, textBoxHeight, RED);
			return;
		}
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		framesCounter = 0;
		DrawRectangleLines(textBoxX, textBoxY, textBoxWidth, textBoxHeight, DARKGRAY);
	}
	void DefaultEndScreen() noexcept {
		DrawText("PRESS ENTER TO CONTINUE", textPosX, 200, fontSize_M, YELLOW);
		DrawText("LEADERBOARD", textPosX_left, 100, fontSize_M, YELLOW);
	}
	void HighscoreScreen(std::string name) noexcept {
		DrawText("NEW HIGHSCORE!", textPosX, 300, fontSize_L, YELLOW);
		DrawText("PLACE MOUSE OVER INPUT BOX!", textPosX, 400, fontSize_S, YELLOW);

		DrawRectangleRec(textBox, LIGHTGRAY);
		HighlightTextbox();
		DrawText(name.c_str(), textBoxX + 5, textBoxY + 8, fontSize_M, MAROON);
		DrawText(TextFormat("INPUT CHARS: %i/%i", name.length(), MAX_INPUT_CHARS - 1), textPosX, 600, fontSize_S, YELLOW);

		if (isStrWithinRange(name, 0, MAX_INPUT_CHARS)) {
			DrawText("PRESS ENTER TO CONTINUE", textPosX, 800, fontSize_M, YELLOW);
		}
		if (!mouseOnText()) { return; }
		if (name.length() >= MAX_INPUT_CHARS) {
			DrawText("Press BACKSPACE to delete chars...", textPosX, 650, fontSize_S, YELLOW);
			return;
		}
		if (!showUnderscoreInTextbox()) { return; }
		DrawText("_", textBoxX + 8 + MeasureText(name.c_str(), fontSize_M), textBoxY + 12, fontSize_M, MAROON);
	}

	void UpdatePlayerAnimation() noexcept {
		playerAnimationTimer += GetFrameTime();
		if (playerAnimationTimer <= animationSpeed) { return; }
		playerAnimationTimer = 0;
		playerActiveTexture == 2 ? playerActiveTexture = 0 : playerActiveTexture++;
	}
};