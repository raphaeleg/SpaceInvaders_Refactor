#include "Leaderboard.h"

void Leaderboard::InsertNewHighScore(std::string name, int score)
{
    PlayerData newData;
    newData.name = name;
    newData.score = score;

    for (int i = 0; i < list.size(); i++)
    {
        if (newData.score > list[i].score)
        {

            list.insert(list.begin() + i, newData);

            list.pop_back();

            i = list.size();
        }
    }
}