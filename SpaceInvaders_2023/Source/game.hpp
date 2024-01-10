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
	float walls_positionY = GetScreenHeightF() - (GetScreenHeightF() / 4.0f);
	float wall_distance = GetScreenWidthF() / (wallCount + 1.0f);

	State gameState = { State::STARTSCREEN };
	float shootTimer = 0;	
	
	Resources resources{};
	Player player{};
	std::vector<Projectile> PlayerProjectiles;
	std::vector<Projectile> AlienProjectiles;
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
	[[nodiscard]] bool IsAlienHit(Vector2 projectilePositione) noexcept;
	[[nodiscard]] bool IsPlayerHit(Vector2 projectilePosition) noexcept;
	[[nodiscard]] bool IsWallHit(Vector2 projectilePosition) noexcept;
	void PlayerShoot();
	void AliensShoot();
	[[nodiscard]] bool IsEndConditionTriggered() noexcept;

	void RenderGameplay() noexcept;


public:
	void Update();
	void Render() noexcept;
};