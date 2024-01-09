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

enum struct State {
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

class Game {
private:
	static constexpr int aliensFormationWidth = 8;
	static constexpr int aliensFormationHeight = 5;
	static constexpr float alienSpacing = 80.0f;
	static constexpr float aliensFormationX = 100.0f;
	static constexpr float aliensFormationY = 50.0f;
	static constexpr int wallCount = 5;


	State gameState = { State::STARTSCREEN };
	float shootTimer = 0;
	std::string draftHighscoreName = "";	
	float wallsY = SCREEN_HEIGHT - 250.0f;
	float wall_distance = SCREEN_WIDTH / (wallCount + 1.0f);
	
	Resources resources{};
	Player player{};
	std::vector<Projectile> Projectiles;
	std::vector<Wall> Walls;
	std::vector<Alien> Aliens;
	Background background{};
	Leaderboard leaderboard{};

	Renderer renderer{};

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
	[[nodiscard]] bool constexpr isValidInput(int key, size_t len) noexcept { return (key > 31) && (key < 126) && (len < MAX_INPUT_CHARS); }
	void updateHighscoreName();

public:
	void Update();
	void Render();
};