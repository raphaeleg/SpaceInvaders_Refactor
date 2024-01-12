#pragma once
#include <string>
#include <vector>
#include "raylib.h"

static constexpr unsigned long long MAX_INPUT_CHARS = 9;
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

	[[nodiscard]] bool constexpr IsValidCharInput(int key) noexcept { return (key > 31) && (key < 126); }

public:
	[[nodiscard]] int GetScore() const noexcept { return score; };
#pragma warning(push)
#pragma warning (disable:26446) // JUSTIFICATION: should be safe as we're only checking the last element in the array using size()
	[[nodiscard]] bool IsNewHighScore() noexcept { return GetScore() > records[records.size()-1].score; };
#pragma warning(pop)
	[[nodiscard]] std::string GetTempHighscoreName() noexcept { return tempHighscoreName; };
	[[nodiscard]] bool IsHighscoreNameValid() noexcept { return GetTempHighscoreName().size() > 0 && GetTempHighscoreName().size() < MAX_INPUT_CHARS; }
	void AddScore() noexcept { score += alienKillScore; }
	void InsertNewHighScore();
	void ResetScore() noexcept { score = 0; }
	void UpdateHighscoreName(bool isMouseOnText);
	void Render(int textPosX, int fontSize) noexcept;
};