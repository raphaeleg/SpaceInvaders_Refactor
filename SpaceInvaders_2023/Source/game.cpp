#include "game.hpp"
#include "Helper.hpp"
#include "Renderer.hpp"
#include <vector>
#include <algorithm>
//#include "gsl_narrow.h"

void Game::Start() {
	player = Player();
	leaderboard.ResetScore();
	SpawnWalls();
	SpawnAliens();
	background = Background();
	gameState = State::GAMEPLAY;
}
void Game::End() noexcept {
	Projectiles.clear();
	Walls.clear();
	Aliens.clear();
	gameState = State::ENDSCREEN;
}
void Game::ShowStartScreen() noexcept {
	gameState = State::STARTSCREEN;
}

void Game::Update() {
	switch (gameState) {
	case State::STARTSCREEN:
		if (IsKeyReleased(KEY_SPACE)) { Start(); }
		break;
	case State::GAMEPLAY:
		if (IsEndConditionTriggered()) {
			End();
			break;
		}
		Renderer::UpdatePlayerAnimation();
		player.Update();
		for (auto& alien : Aliens) { alien.Update(); }
		if (Aliens.size() < 1) { SpawnAliens(); }

		CheckProjectileHit();

		if (IsKeyPressed(KEY_SPACE)) { PlayerShoot(); }
		AliensShoot();

		ClearDeadEntities();
		break;
	case State::ENDSCREEN:
		if (!leaderboard.IsNewHighScore()) {
			if (IsKeyReleased(KEY_ENTER)) { ShowStartScreen(); }
			break;
		}
		updateHighscoreName();
		if (IsKeyReleased(KEY_ENTER) && isStrWithinRange(draftHighscoreName, 0, MAX_INPUT_CHARS)) {
			leaderboard.InsertNewHighScore(draftHighscoreName);
			draftHighscoreName = "";
		}
		break;
	default:
		break;
	}
}

void Game::Render() {
	switch (gameState) {
	case State::STARTSCREEN:
		Renderer::StartScreen();
		break;
	case State::GAMEPLAY:
		RenderGameplay();
		break;
	case State::ENDSCREEN:
		if (leaderboard.IsNewHighScore()) {
			Renderer::HighscoreScreen(draftHighscoreName);
			break;
		}
		Renderer::DefaultEndScreen();
		leaderboard.Render(Renderer::textPosX_left, Renderer::fontSize_M);
		break;
	default:
		break;
	}
}

void Game::SpawnWalls() {
	for (int i = 1; i < wallCount + 1; i++) {
		Walls.push_back(Wall(wall_distance * i, wallsY));
	}
}

void Game::SpawnAliens() {
	for (int row = 0; row < aliensFormationHeight; row++) {
		for (int col = 0; col < aliensFormationWidth; col++) {
			Aliens.push_back(
				Alien(aliensFormationX + 450.0f + (col * alienSpacing), aliensFormationY + (row * alienSpacing))
			);
		}
	}
}

bool Game::IsEndConditionTriggered() noexcept {
	bool hasAlienReachedWalls = false;
	if (Aliens.size() >= 1) {
		hasAlienReachedWalls = Aliens.at(Aliens.size() - 1).HasReachedYPosition(SCREEN_HEIGHT_INT - static_cast<int>(PLAYER_BASE_HEIGHT));
	}
	return IsKeyReleased(KEY_Q) || player.IsDead() || hasAlienReachedWalls;
}

void Game::CheckProjectileHit() noexcept {
	const Vector2 playerPos = { player.GetPosition(), PLAYER_BASE_HEIGHT };
	const Vector2 cornerPos = { 0, PLAYER_BASE_HEIGHT };
	const float offset = lineLength(playerPos, cornerPos) * -1;
	background.Update(offset / 15);

	for (auto& projectile : Projectiles) {
		projectile.Update();
		if (IsWallHit(projectile)) {
			projectile.Hit();
			continue;
		};
		if (projectile.IsPlayerProjectile()) {
			if (AreAliensHit(projectile)) { projectile.Hit(); }
			continue;
		}
		if (IsPlayerHit(projectile)) { projectile.Hit(); }
	}
}
bool Game::AreAliensHit(Projectile projectile) noexcept {
	for (auto& alien : Aliens) {
		if (!CheckCollision(alien.GetPosition(), ALIEN_RADIUS, projectile.GetPosition())) { continue; }
		alien.Kill();
		leaderboard.AddScore();
		return true;
	}
	return false;
}
bool Game::IsPlayerHit(Projectile projectile) noexcept {
	const Vector2 calcPlayerPos = { player.GetPosition(), SCREEN_HEIGHT - PLAYER_BASE_HEIGHT };
	if (!CheckCollision(calcPlayerPos, PLAYER_RADIUS, projectile.GetPosition())) { return false; }
	player.DecreaseHealth();
	return true;
}
bool Game::IsWallHit(Projectile projectile) noexcept {
	for (auto& wall : Walls) {
		if (!CheckCollision(wall.GetPosition(), WALL_RADIUS, projectile.GetPosition())) { continue; }
		wall.DecreaseHealth();
		return true;
	}
	return false;
}

void Game::PlayerShoot() {
	const Vector2 projectilePos = { player.GetPosition(), SCREEN_HEIGHT - 130 };
	Projectiles.push_back(Projectile(projectilePos, true));
}
void Game::AliensShoot() {
	shootTimer += 1;
	if (shootTimer < 60) { return; }
	shootTimer = 0;

	const int randomAlienIndex = Aliens.size() > 1 ? rand() % Aliens.size() : 0;
	Vector2 projectilePos = Aliens.at(randomAlienIndex).GetPosition();
	projectilePos.y += 40;
	Projectiles.push_back(Projectile(projectilePos, false));
}
void Game::ClearDeadEntities() {
	Projectiles.erase(std::remove_if(Projectiles.begin(), Projectiles.end(), [](auto v) noexcept { return v.IsDead(); }), Projectiles.end());
	Walls.erase(std::remove_if(Walls.begin(), Walls.end(), [](auto v) noexcept { return v.IsDead(); }), Walls.end());
	Aliens.erase(std::remove_if(Aliens.begin(), Aliens.end(), [](auto v) noexcept { return v.IsDead(); }), Aliens.end());
}

void Game::RenderGameplay() noexcept {
	background.Render();
	player.Render(resources.GetShip(Renderer::playerActiveTexture));
	std::ranges::for_each(Projectiles, [&](auto v) noexcept { v.Render(resources.GetProjectile()); });
	std::ranges::for_each(Walls, [&](auto v) noexcept { v.Render(resources.GetWall()); });
	std::ranges::for_each(Aliens, [&](auto v) noexcept { v.Render(resources.GetAlien()); });

	DrawText(TextFormat("Score: %i", leaderboard.GetScore()), 50, 20, 40, YELLOW);
	DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, 40, YELLOW);
}
void Game::updateHighscoreName() {
	const int key = GetCharPressed();
	if (isValidInput(key, std::size(draftHighscoreName))) {
		draftHighscoreName.push_back(static_cast<char>(key));
	}
	if (IsKeyPressed(KEY_BACKSPACE) && !draftHighscoreName.empty()) {
		draftHighscoreName.pop_back();
	}
}