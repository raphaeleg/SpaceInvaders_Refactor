#pragma once
#include <string>
#include <vector>
#include "raylib.h"

static constexpr int MAX_INPUT_CHARS = 9;
struct PlayerData {
	std::string name;
	int score;
};

class Leaderboard {
private:
	std::string tempHighscoreName = "";
	int score = 0;
	static constexpr int alienKillScore = 100;
	std::vector<PlayerData> records = { {"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100} };

	[[nodiscard]] bool constexpr isValidInput(int key) noexcept { return (key > 31) && (key < 126); }
	[[nodiscard]] bool tempHighscoreNameIsValid() noexcept { return tempHighscoreName.size() > -1 && tempHighscoreName.size() < MAX_INPUT_CHARS + 1; }

public:
	[[nodiscard]] int GetScore() const noexcept { return score; };
	[[nodiscard]] bool IsNewHighScore() noexcept { return score > records.at(4).score; };
	[[nodiscard]] std::string GetTempHighscoreName() noexcept { return tempHighscoreName; };
	void AddScore() noexcept { score += alienKillScore; }
	void InsertNewHighScore();
	void ResetScore() noexcept { score = 0; }
	void updateHighscoreName();
	void Render(int textPosX, int fontSize) noexcept;
};