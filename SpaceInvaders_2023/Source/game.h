#pragma once
#include "raylib.h"
#include <vector>
#include "Resources.h"
#include <string>
#include "Player.h"
#include "Wall.h"
#include "Alien.h"
#include "Projectile.h"
#include "Background.h"
#include "Leaderboard.h"


enum struct State
{
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

struct Game
{
	State gameState = {};
	int score;
	int wallCount = 5;
	float wallsY = GetScreenHeight() - 250.0f;
	float wall_distance = GetScreenWidth() / (wallCount + 1.0f);
	float shootTimer = 0;
	Rectangle rec = { 0, 0 ,0 ,0 };

	int formationWidth = 8;
	int formationHeight = 5;
	int alienSpacing = 80;
	int formationX = 100;
	int formationY = 50;

	bool newHighScore = false;

	void Start();
	void End();

	void Continue();

	void Update();
	void Render();

	void SpawnAliens();

	std::vector<OwnTexture> shipTextures{
		OwnTexture("Assets/Ship1.png"),
		OwnTexture("Assets/Ship2.png"),
		OwnTexture("Assets/Ship3.png"),
	};
	OwnTexture alienTexture{ "Assets/Alien.png" };
	OwnTexture barrierTexture{ "Assets/Barrier.png" };
	OwnTexture laserTexture{ "Assets/Laser.png" };

	Player player;
	std::vector<Projectile> Projectiles;
	std::vector<Wall> Walls;
	std::vector<Alien> Aliens;
	Background background;
	Leaderboard leaderboard;

	Vector2 playerPos;
	Vector2 alienPos;
	Vector2 cornerPos;
	float offset;

	char name[9 + 1] = "\0";
	int letterCount = 0;

	Rectangle textBox = { 600, 500, 225, 50 };
	bool mouseOnText = false;

	int framesCounter = 0;
};