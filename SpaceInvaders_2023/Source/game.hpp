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
	float alienShootTimer = 0;
	static constexpr float SHOOT_DELAY = 60;

	static constexpr int wallCount = 5;
	float wallsPositionY = GetScreenHeightF() - 250.0f;
	float wallDistance = GetScreenWidthF() / (wallCount + 1.0f);
	static constexpr int aliensFormationWidth = 8;
	static constexpr int aliensFormationHeight = 5;
	static constexpr float alienSpacing = 80.0f;
	static constexpr float aliensFormationX = 550.0f;
	static constexpr float aliensFormationY = 50.0f;

	Resources resources{};
	Renderer renderer{};
	Player player{};
	Leaderboard leaderboard{};
	Background background{};
	std::vector<Projectile> PlayerProjectiles;
	std::vector<Projectile> AlienProjectiles;
	std::vector<Wall> Walls;
	std::vector<Alien> Aliens;

	void Start();
	void End() noexcept;
	void ShowStartScreen() noexcept;
	void SpawnAliens();
	void SpawnWalls();

	// TODO: move all these functions to a separate file of sorts
	void HandleProjectileHit() noexcept;
	void RemoveOutOfScreenProjectiles(std::vector<Projectile> &projectiles) noexcept;
	void RemoveWallHitProjectiles(std::vector<Projectile> &projectiles) noexcept;
	void RemoveAlienHitProjectiles(std::vector<Projectile> &projectiles) noexcept;
	void RemovePlayerHitProjectiles(std::vector<Projectile> &projectiles) noexcept;
	[[nodiscard]] bool IsAlienHit(Vector2 projectilePositione) noexcept;
	[[nodiscard]] bool IsPlayerHit(Vector2 projectilePosition) noexcept;
	[[nodiscard]] bool IsWallHit(Vector2 projectilePosition) noexcept;
	
	void PlayerShoot();
	void AliensShoot();
	[[nodiscard]] bool IsEndGameConditionTriggered() noexcept;

	void RenderGameplay() const noexcept;
public:
	void Update();
	void Render() noexcept;
};