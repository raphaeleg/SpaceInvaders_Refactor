#include "Renderer.hpp"

void Renderer::StartScreen() noexcept {
	DrawText("SPACE INVADERS", 200, 100, fontSize_XL, YELLOW);
	DrawText("PRESS SPACE TO BEGIN", 200, 350, fontSize_M, YELLOW);
}
void Renderer::GameplayText(int score, int lives) noexcept {
	DrawText(TextFormat("Score: %i", score), textPosX_left, 20, fontSize_M, YELLOW);
	DrawText(TextFormat("Lives: %i", lives), textPosX_left, 70, fontSize_M, YELLOW);
}
void Renderer::HighlightTextbox() noexcept {
	if (mouseOnText()) {
		SetMouseCursor(MOUSE_CURSOR_IBEAM);
		framesCounter++; // TODO: find way to make this func const if possible. it bubbles up.
		DrawRectangleLines(textBoxX(), textBoxY(), textBoxWidth(), textBoxHeight(), RED);
		return;
	}
	SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	framesCounter = 0;
	DrawRectangleLines(textBoxX(), textBoxY(), textBoxWidth(), textBoxHeight(), DARKGRAY);
}
void Renderer::DefaultEndScreen() noexcept {
	DrawText("PRESS ENTER TO CONTINUE", textPosX, 200, fontSize_M, YELLOW);
	DrawText("LEADERBOARD", textPosX_left, 100, fontSize_M, YELLOW);
}
void Renderer::HighscoreScreen(std::string name, int maxChars) noexcept {
	DrawText("NEW HIGHSCORE!", textPosX, 300, fontSize_L, YELLOW);
	DrawText("PLACE MOUSE OVER INPUT BOX!", textPosX, 400, fontSize_S, YELLOW);

	DrawRectangleRec(textBox, LIGHTGRAY);
	HighlightTextbox();
	DrawText(name.data(), textBoxX() + 5, textBoxY() + 8, fontSize_M, MAROON);
	DrawText(TextFormat("INPUT CHARS: %i/%i", name.size(), maxChars - 1), textPosX, 600, fontSize_S, YELLOW);

	if (name.size() < maxChars) {
		DrawText("PRESS ENTER TO CONTINUE", textPosX, 800, fontSize_M, YELLOW);
	}
	if (!mouseOnText()) { return; }
	if (name.length() >= maxChars) {
		DrawText("Press BACKSPACE to delete chars...", textPosX, 650, fontSize_S, YELLOW);
		return;
	}
	if (!ShowUnderscoreInTextbox()) { return; }
	DrawText("_", textBoxX() + 8 + MeasureText(name.data(), fontSize_M), textBoxY() + 12, fontSize_M, MAROON);
}

void Renderer::UpdatePlayerAnimation() noexcept {
	playerAnimationTimer += GetFrameTime();
	if (playerAnimationTimer <= animationSpeed) { return; }
	playerAnimationTimer = 0;
	playerActiveTexture == 2 ? playerActiveTexture = 0 : playerActiveTexture++;
}