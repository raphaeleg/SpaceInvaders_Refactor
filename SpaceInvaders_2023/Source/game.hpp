#pragma once
#include "raylib.h"
#include "Resources.hpp"
#include "Renderer.hpp"
#include "Player.hpp"
#include "Wall.hpp"
#include "Alien.hpp"
#include "Projectile.hpp"
#include "Background.hpp"
#include "Leaderboard.hpp"
#include "Helper.hpp"
#include <vector>
#include <string>
#include <chrono>
#include <time.h>    

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
	float wallsY = GetScreenHeightF() - 250.0f;
	float wall_distance = GetScreenWidthF() / (wallCount + 1.0f);
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
	Renderer renderer = Renderer();

	void Start();
	void End() noexcept;
	void ShowStartScreen() noexcept;
	void SpawnAliens();
	void SpawnWalls();

	void HandleProjectileHit() noexcept;
	[[nodiscard]] bool HandledAlienHit(Vector2 projectilePositione) noexcept;
	[[nodiscard]] bool HandledPlayerHit(Vector2 projectilePosition) noexcept;
	[[nodiscard]] bool HandledWallHit(Vector2 projectilePosition) noexcept;
	void PlayerShoot();
	void AliensShoot();
	void ClearDeadEntities();
	[[nodiscard]] bool IsEndConditionTriggered() noexcept;

	void RenderGameplay() noexcept;
	[[nodiscard]] bool constexpr isValidInput(int key, size_t len) noexcept { return (key >= 32) && (key <= 125) && (len < MAX_INPUT_CHARS); }
	void updateHighscoreName();

	unsigned int frameCounter = 0;
	unsigned int savedframeCounter = 0;
	unsigned __int64 frameStartTime = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	void UpdateBenchmark() noexcept;
public:
	void Update();
	void Render();
};