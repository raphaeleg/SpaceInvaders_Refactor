#include "game.hpp"
#include <vector>
#include <algorithm>

void Game::Start() {
	player = Player();
	leaderboard.ResetScore();
	SpawnWalls();
	SpawnAliens();
	background = Background();
	gameState = State::GAMEPLAY;
}
void Game::End() noexcept {
	PlayerProjectiles.clear();
	AlienProjectiles.clear();
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
		renderer.UpdatePlayerAnimation();
		player.Update();
		for (auto& alien : Aliens) { alien.Update(); }
		if (Aliens.size() < 1) { SpawnAliens(); }

		HandleProjectileHit();
		if (IsKeyPressed(KEY_SPACE)) { PlayerShoot(); }
		AliensShoot();
		PlayerProjectiles.erase(std::remove_if(PlayerProjectiles.begin(), PlayerProjectiles.end(), [](auto v) noexcept { return v.IsDead(); }), PlayerProjectiles.end());
		AlienProjectiles.erase(std::remove_if(AlienProjectiles.begin(), AlienProjectiles.end(), [](auto v) noexcept { return v.IsDead(); }), AlienProjectiles.end());
		break;
	case State::ENDSCREEN:
		if (!leaderboard.IsNewHighScore()) {
			if (IsKeyReleased(KEY_ENTER)) { ShowStartScreen(); }
			break;
		}
		leaderboard.updateHighscoreName();
		if (IsKeyReleased(KEY_ENTER)) { 
			leaderboard.InsertNewHighScore(); 
		}
		break;
	default:
		break;
	}
}

void Game::Render() noexcept {
	switch (gameState) {
	case State::STARTSCREEN:
		renderer.StartScreen();
		break;
	case State::GAMEPLAY:
		RenderGameplay();
		break;
	case State::ENDSCREEN:
		if (leaderboard.IsNewHighScore()) {
			renderer.HighscoreScreen(leaderboard.GetTempHighscoreName());
			break;
		}
		renderer.DefaultEndScreen();
		leaderboard.Render(textPosX_left, fontSize_M);
		break;
	default:
		break;
	}
}

void Game::SpawnWalls() {
	for (int i = 0; i < wallCount; i++) {
		Walls.emplace_back(wall_distance*(i+1), walls_positionY);
	}
}
void Game::SpawnAliens() {
	for (int row = 0; row < aliensFormationHeight; row++) {
		for (int col = 0; col < aliensFormationWidth; col++) {
			const auto pos = Vector2(aliensFormationX + 450.0f + (col * alienSpacing), aliensFormationY + (row * alienSpacing));			
			Aliens.emplace_back(pos);
		}
	}
}

bool Game::IsEndConditionTriggered() noexcept {
	bool hasAlienReachedWalls = false;
	if (Aliens.size() >= 1) {
		#pragma warning(push)
		#pragma warning(disable:26446) // JUSTIFICATION: size checking in place
		hasAlienReachedWalls = Aliens[Aliens.size() - 1].HasReachedYPosition(GetScreenHeight() - PLAYER_BASE_HEIGHT);
		#pragma warning(pop)
	}
	return IsKeyReleased(KEY_Q) || player.IsDead() || hasAlienReachedWalls;
}

void Game::HandleProjectileHit() noexcept {
	for (auto& projectile : PlayerProjectiles) {
		projectile.Update();

		const Vector2 projectilePos = projectile.GetPosition();
		bool projectileKill = false;

		if (projectilePos.y < 0 || projectilePos.y > GetScreenWidth()) { projectileKill = true; }
		else if (IsWallHit(projectilePos)) { projectileKill = true; }
		else if (IsAlienHit(projectilePos)) { projectileKill = true; }
		if (projectileKill) { 
			projectile.Kill();
		}
	}
	for (auto& projectile : AlienProjectiles) {
		projectile.Update();

		const Vector2 projectilePos = projectile.GetPosition();
		bool projectileKill = false;

		if (projectilePos.y < 0 || projectilePos.y > GetScreenWidth()) { projectileKill = true; }
		else if (IsWallHit(projectilePos)) { projectileKill = true; }
		else if (IsPlayerHit(projectilePos)) { projectileKill = true; }
		if (projectileKill) {
			projectile.Kill();
		}
	}
}
bool Game::IsAlienHit(Vector2 projectilePosition) noexcept {
	const auto findAlienHit = std::ranges::find_if(Aliens, [&](auto alien) noexcept {
		return CheckCollision(alien.GetPosition(), ALIEN_RADIUS, projectilePosition);
		});
	if (findAlienHit == Aliens.end()) { return false; }
	Aliens.erase(findAlienHit);
	leaderboard.AddScore();
	return true;
}
bool Game::IsPlayerHit(Vector2 projectilePosition) noexcept {	
	const Vector2 calcPlayerPos = { player.GetPositionX(), GetScreenHeightF() - PLAYER_BASE_HEIGHT };
	if (!CheckCollision(calcPlayerPos, PLAYER_RADIUS, projectilePosition)) { return false; }
	player.DecreaseHealth();
	return true;
}
bool Game::IsWallHit(Vector2 projectilePosition) noexcept {
	const auto wall = std::ranges::find_if(Walls, [&](auto wall) noexcept {
		return CheckCollision(wall.GetPosition(), WALL_RADIUS, projectilePosition);
		});
	if (wall == Walls.end()) { return false; }
	wall->DecreaseHealth();
	if (wall->IsDead()) {
		Walls.erase(wall);
	}
	return true;
}

void Game::PlayerShoot() {
	const Vector2 projectilePos = { player.GetPositionX(), GetScreenHeightF() - 130 };
	PlayerProjectiles.emplace_back(Projectile(projectilePos, true));
}
void Game::AliensShoot() {
	shootTimer += 1;
	if (shootTimer < 60) { return; }
	shootTimer = 0;

	const int randomAlienIndex = Aliens.size() > 1 ? rand() % Aliens.size() : 0;
	#pragma warning(push)
	#pragma warning(disable:26446) // JUSTIFICATION: size checking in place
	Vector2 projectilePos = Aliens[randomAlienIndex].GetPosition();
	#pragma warning(pop)
	projectilePos.y += 40;
	AlienProjectiles.emplace_back(Projectile(projectilePos, false));
}

void Game::RenderGameplay() noexcept {
	background.Render(player.GetPositionX());
	player.Render(resources.GetShip(renderer.GetPlayerActiveTexture()));
	std::ranges::for_each(PlayerProjectiles, [&](auto v) noexcept { v.Render(resources.GetProjectile()); });
	std::ranges::for_each(AlienProjectiles, [&](auto v) noexcept { v.Render(resources.GetProjectile()); });
	std::ranges::for_each(Walls, [&](auto v) noexcept { v.Render(resources.GetWall()); });
	std::ranges::for_each(Aliens, [&](auto v) noexcept { v.Render(resources.GetAlien()); });

	DrawText(TextFormat("Score: %i", leaderboard.GetScore()), 50, 20, 40, YELLOW);
	DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, 40, YELLOW);
}