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
	float shootTimer = 0;
	std::string draftHighscoreName = "";

	static constexpr int wallCount = 5;
	float wallsY = SCREEN_HEIGHT - 250.0f;
	float wall_distance = SCREEN_WIDTH / (wallCount + 1.0f);
	static constexpr int aliensFormationWidth = 8;
	static constexpr int aliensFormationHeight = 5;
	static constexpr float alienSpacing = 80.0f;
	static constexpr float aliensFormationX = 100.0f;
	static constexpr float aliensFormationY = 50.0f;

	Player player;
	std::vector<Projectile> Projectiles;
	std::vector<Wall> Walls;
	std::vector<Alien> Aliens;
	Background background;
	Leaderboard leaderboard = Leaderboard();
	Resources resources = Resources();

	void Start();
	void End() noexcept;
	void ShowStartScreen() noexcept;
	void SpawnAliens();
	void SpawnWalls();

	void CheckProjectileHit() noexcept;
	[[nodiscard]] bool AreAliensHit(Projectile projectile) noexcept;
	[[nodiscard]] bool IsPlayerHit(Projectile projectile) noexcept;
	[[nodiscard]] bool IsWallHit(Projectile projectile) noexcept;
	void PlayerShoot();
	void AliensShoot();
	void ClearDeadEntities();
	[[nodiscard]] bool IsEndConditionTriggered() noexcept;

	void RenderGameplay() noexcept;
	[[nodiscard]] bool constexpr isValidInput(int key, size_t len) noexcept { return (key >= 32) && (key <= 125) && (len < MAX_INPUT_CHARS); }
	void updateHighscoreName();

public:
	void Update();
	void Render();
};