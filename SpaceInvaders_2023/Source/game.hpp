#pragma once
#include "raylib.h"
#include "Resources.hpp"
#include "Player.hpp"
#include "Wall.hpp"
#include "Alien.hpp"
#include "Projectile.hpp"
#include "Background.hpp"
#include "Leaderboard.hpp"
#include "Helper.hpp"
#include <vector>
#include <string>

enum struct State {
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

class Game {
private:
	State gameState = { State::STARTSCREEN };
	int score = 0;
	static constexpr int scoreAddifyer = 100;
	float shootTimer = 0;
	bool newHighScore = false;

	static constexpr int wallCount = 5;
	float wallsY = SCREEN_HEIGHT - 250.0f;
	float wall_distance = SCREEN_WIDTH / (wallCount + 1.0f);
	static constexpr int aliensFormationWidth = 8;
	static constexpr int aliensFormationHeight = 5;
	static constexpr float alienSpacing = 80.0f;
	static constexpr float aliensFormationX = 100.0f;
	static constexpr float aliensFormationY = 50.0f;

	Resources resources = Resources();
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

	void Start();
	void End() noexcept;
	void ShowStartScreen() noexcept;
	void SpawnAliens();
	void SpawnWalls();

	void CheckProjectileHit() noexcept;
	void PlayerShoot();
	void AliensShoot();
	void ClearDeadEntities();
	bool IsEndConditionTriggered();

	void RenderGameplay() noexcept;

public:
	void Update();
	void Render();
};