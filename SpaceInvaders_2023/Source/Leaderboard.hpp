#pragma once
#include <string>
#include <vector>

static constexpr int MAX_INPUT_CHARS = 9;

struct PlayerData
{
    std::string name;
    int score;
};

class Leaderboard {
public:
    std::vector<PlayerData> list = { {"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100} };
    [[nodiscard]] bool CheckNewHighScore(int score) noexcept { return score > list.at(4).score; };
    void InsertNewHighScore(std::string name, int score);
};