#include "game.hpp"
#include "Helper.hpp"
#include "Renderer.hpp"
#include <vector>
#include <algorithm>

void Game::Start() {
	SpawnWalls();
	player = Player();
	SpawnAliens();
	background = Background();
	leaderboard.ResetScore();
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
		if (IsKeyReleased(KEY_SPACE)) {
			Start();
		}
		break;

	case State::GAMEPLAY:
		if (IsEndConditionTriggered()) {
			End();
			break;
		}
		Renderer::UpdatePlayerAnimation();
		player.Update();
		for (auto& alien : Aliens) {
			alien.Update();
		}
		if (Aliens.size() < 1) {
			SpawnAliens();
		}

		CheckProjectileHit();

		if (IsKeyPressed(KEY_SPACE)) {
			PlayerShoot();
		}
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

bool Game::IsEndConditionTriggered() {
	bool hasAlienReached = false;
	if (Aliens.size() >= 1) {
		hasAlienReached = Aliens.at(0).HasReachedYPosition(SCREEN_HEIGHT_INT - static_cast<int>(PLAYER_BASE_HEIGHT));
	}
	return IsKeyReleased(KEY_Q) || player.IsDead() || hasAlienReached;
}

void Game::CheckProjectileHit() noexcept {
	playerPos = { player.GetPosition(), PLAYER_BASE_HEIGHT };
	cornerPos = { 0, PLAYER_BASE_HEIGHT };
	offset = lineLength(playerPos, cornerPos) * -1;
	background.Update(offset / 15);

	for (auto& projectile : Projectiles)
	{
		projectile.Update();
		if (projectile.IsPlayerProjectile()) {
			for (auto& alien : Aliens) {
				if (CheckCollision(alien.GetPosition(), ALIEN_RADIUS, projectile.lineStart, projectile.lineEnd)) {
					projectile.Hit();
					alien.Kill();
					leaderboard.AddScore();
				}
			}
		}

		else {
			if (CheckCollision({ player.GetPosition(), SCREEN_HEIGHT - PLAYER_BASE_HEIGHT }, PLAYER_RADIUS, projectile.lineStart, projectile.lineEnd)) {
				projectile.Hit();
				player.DecreaseHealth();
			}
		}

		for (auto& wall : Walls) {
			if (CheckCollision(wall.GetPosition(), WALL_RADIUS, projectile.lineStart, projectile.lineEnd)) {
				projectile.Hit();
				wall.DecreaseHealth();
			}
		}
	}
}

void Game::PlayerShoot() {
	const Vector2 projectilePos = { player.GetPosition(), SCREEN_HEIGHT - 130 };
	Projectiles.push_back(Projectile(projectilePos, true));
}

void Game::AliensShoot() {
	shootTimer += 1;

	if (shootTimer < 60) { return; }
	const int randomAlienIndex = Aliens.size() > 1 ? rand() % Aliens.size() : 0;

	Vector2 projectilePos = Aliens.at(randomAlienIndex).GetPosition();
	projectilePos.y += 40;
	Projectiles.push_back(Projectile(projectilePos, false));

	shootTimer = 0;
}

void Game::ClearDeadEntities() {
	for (int i = 0; i < Projectiles.size(); i++) {
		if (Projectiles.at(i).IsDead()) {
			Projectiles.erase(Projectiles.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < Aliens.size(); i++) {
		if (Aliens.at(i).IsDead()) {
			Aliens.erase(Aliens.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < Walls.size(); i++) {
		if (Walls.at(i).IsDead()) {
			Walls.erase(Walls.begin() + i);
			i--;
		}
	}
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