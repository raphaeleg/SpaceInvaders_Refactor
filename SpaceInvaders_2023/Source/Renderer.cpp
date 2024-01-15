#include "Renderer.hpp"

void Renderer::StartScreen() const noexcept {
	constexpr int _textPosX = 200;
	constexpr int padding = static_cast<int>(textPadding*3.5);
	DrawText("SPACE INVADERS", _textPosX, textPosY, fontSize_XL, fontColor);
	DrawText("PRESS SPACE TO BEGIN", _textPosX, textPosY + padding, fontSize_M, fontColor);
}
void Renderer::GameplayText(int score, int lives) const noexcept {
	constexpr int _textPosY = 20;
	constexpr int padding = static_cast<int>(textPadding*0.5);
	DrawText(TextFormat("Score: %i", score), textPosX_left, _textPosY, fontSize_M, fontColor);
	DrawText(TextFormat("Lives: %i", lives), textPosX_left, _textPosY + padding, fontSize_M, fontColor);
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
void Renderer::DefaultEndScreen() const noexcept {
	DrawText("LEADERBOARD", textPosX_left, textPosY, fontSize_M, fontColor);
	DrawText("PRESS ENTER TO CONTINUE", textPosX, textPosY + textPadding, fontSize_M, fontColor);
}
void Renderer::HighscoreScreen(std::string name, int maxChars) noexcept {
	constexpr int _textPosY = 300;
	DrawText("NEW HIGHSCORE!", textPosX, _textPosY, fontSize_L, fontColor);
	DrawText("PLACE MOUSE OVER INPUT BOX!", textPosX, _textPosY + textPadding, fontSize_S, fontColor);

	DrawRectangleRec(textBox, LIGHTGRAY);
	HighlightTextbox();
	DrawText(name.data(), textBoxX() + 5, textBoxY() + 8, fontSize_M, MAROON);
	DrawText(TextFormat("INPUT CHARS: %i/%i", name.size(), maxChars - 1), textPosX, (_textPosY + textPadding * 3), fontSize_S, fontColor);

	if (name.size() > 0 && name.size() < maxChars) {
		DrawText("PRESS ENTER TO CONTINUE", textPosX, (_textPosY + textPadding * 4), fontSize_M, fontColor);
	}
	if (!mouseOnText()) { 
		return; 
	}
	else if (name.size() == maxChars) {
		DrawText("Press BACKSPACE to delete chars...", textPosX, (_textPosY + static_cast<int>(textPadding * 3.5)), fontSize_S, fontColor);
		return;
	}
	else if (!ShowUnderscoreInTextbox()) {
		return; 
	}
	DrawText("_", textBoxX() + 8 + MeasureText(name.data(), fontSize_M), textBoxY() + 12, fontSize_M, MAROON);
}

void Renderer::UpdatePlayerAnimation() noexcept {
	playerAnimationTimer += GetFrameTime();
	if (!(playerAnimationTimer > animationSpeed)) {
		return; 
	}
	playerAnimationTimer = 0;
	playerActiveTexture == 2 ? playerActiveTexture = 0 : playerActiveTexture++;
}