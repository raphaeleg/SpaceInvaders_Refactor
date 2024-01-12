#include "Leaderboard.hpp"
#include "Helper.hpp"
#include "raylib.h"
#include <algorithm>

void Leaderboard::InsertNewHighScore() {
	if (!IsHighscoreNameValid()) { return; }
	PlayerData newData{ tempHighscoreName, score };
	records.emplace_back(newData);
	std::ranges::sort(records, std::ranges::greater{}, &PlayerData::score);
	records.erase(records.end() - 1);
	ResetScore();
	tempHighscoreName = "";
}

void Leaderboard::UpdateHighscoreName(bool isMouseOnText) {
	if (!isMouseOnText) { return; }
	const auto key = GetCharPressed();
	if (IsValidCharInput(key) && tempHighscoreName.size() < MAX_INPUT_CHARS) {
#pragma warning(push)
#pragma warning(disable:26472) // JUSTIFICATION: data loss is acceptable, as we're checking input before converting
		const auto keyStr = static_cast<char>(key);
#pragma warning(pop)
		tempHighscoreName += keyStr;
	}
	if (IsKeyPressed(KEY_BACKSPACE) && !tempHighscoreName.empty()) {
		tempHighscoreName.pop_back();
	}
}

void Leaderboard::Render(int textPosX, int fontSize) noexcept {
	for (int i = 0; i < records.size(); ++i) {
		const int posY = 140 + i * 40;
#pragma warning(push)
#pragma warning(disable:26446) // JUSTIFICATION: iterator checking in place
		DrawText(records[i].name.data(), textPosX, posY, fontSize, YELLOW);
		DrawText(TextFormat("%i", records[i].score), textPosX + 300, posY, fontSize, YELLOW);
#pragma warning(pop)
	}
}