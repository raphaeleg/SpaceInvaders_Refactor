#include "game.hpp"
#include "Helper.hpp"
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
	UpdateBenchmark();
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
		background.Update(player.GetPositionX());

		HandleProjectileHit();
		if (IsKeyPressed(KEY_SPACE)) { PlayerShoot(); }
		AliensShoot();
		break;
	case State::ENDSCREEN:
		if (!leaderboard.IsNewHighScore()) {
			if (IsKeyReleased(KEY_ENTER)) { ShowStartScreen(); }
			break;
		}
		leaderboard.updateHighscoreName(renderer.mouseOnText());
		if (IsKeyReleased(KEY_ENTER)) {
			leaderboard.InsertNewHighScore();
		}
		break;
	default:
		break;
	}
}

void Game::Render() {
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
	DrawText(TextFormat("Benchmark: %i", savedframeCounter), 50, 120, 40, RED);
}

void Game::SpawnWalls() {
	for (int i = 0; i < wallCount; i++) {
		const auto pos = Vector2{wall_distance * (i+1), wallsY};
		Walls.emplace_back(Wall(pos));
	}
}
void Game::SpawnAliens() {
	for (int row = 0; row < aliensFormationHeight; row++) {
		for (int col = 0; col < aliensFormationWidth; col++) {
			const float posX = aliensFormationX + (col * alienSpacing);
			const float posY = aliensFormationY + (row * alienSpacing);
			Aliens.emplace_back(Alien({posX,posY}));
		}
	}
}

bool Game::IsEndConditionTriggered() noexcept {
	bool hasAlienReachedWalls = false;
	if (Aliens.size() > 0) { // TODO: is this check necessary?
		hasAlienReachedWalls = Aliens.at(Aliens.size() - 1).HasReachedYPosition(player.GetPositionY());
	}
	return IsKeyReleased(KEY_Q) || player.IsDead() || hasAlienReachedWalls;
}

void Game::HandleProjectileHit() noexcept {
	RemoveOutOfScreenProjectiles(PlayerProjectiles);
	RemoveOutOfScreenProjectiles(AlienProjectiles);
	RemoveWallHitProjectiles(PlayerProjectiles);
	RemoveWallHitProjectiles(AlienProjectiles);
	RemoveAlienHitProjectiles(PlayerProjectiles);
	RemovePlayerHitProjectiles(AlienProjectiles);
}
void Game::RemoveOutOfScreenProjectiles(std::vector<Projectile>& v) noexcept {
    v.erase(std::remove_if(v.begin(), v.end(), [](const auto& projectile) noexcept {
        return projectile.isOutOfScreen();
    }), v.end());
}
void Game::RemoveWallHitProjectiles(std::vector<Projectile>& v) noexcept {
    v.erase(std::remove_if(v.begin(), v.end(), [](const auto& projectile) noexcept {
        return HandledWallHit(projectile.GetPosition());
    }), v.end());
}
void Game::RemoveAlienHitProjectiles(std::vector<Projectile>& v) noexcept {
    v.erase(std::remove_if(v.begin(), v.end(), [](const auto& projectile) noexcept {
        return HandledAlienHit(projectile.GetPosition());
    }), v.end());
}
void Game::RemovePlayerHitProjectiles(std::vector<Projectile>& v) noexcept {
    v.erase(std::remove_if(v.begin(), v.end(), [](const auto& projectile) noexcept {
        return HandledPlayerHit(projectile.GetPosition());
    }), v.end());
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
	if (!CheckCollision(player.GetPosition(), PLAYER_RADIUS, projectilePosition)) { return false; }
	player.DecreaseHealth();
	return true;
}
bool Game::IsWallHit(Vector2 projectilePosition) noexcept {
	const auto findWallHit = std::ranges::find_if(Walls, [&](auto wall) noexcept {
		return CheckCollision(wall.GetPosition(), WALL_RADIUS, projectilePosition);
		});
	if (findWallHit == Walls.end()) { return false; }
	findWallHit->DecreaseHealth();
	if (findWallHit->IsDead()) { Walls.erase(findWallHit); }
	return true;
}


void Game::PlayerShoot() {
	const auto projectilePos = Vector2{ player.GetPositionX(), GetScreenHeightF() - 130 }; // TODO: find what 130 represents
	PlayerProjectiles.emplace_back(Projectile(projectilePos, true));
}
void Game::AliensShoot() {
	shootTimer += 1;
	if (shootTimer < SHOOT_DELAY) { return; }
	shootTimer = 0;

	const int randomAlienIndex = Aliens.size() > 1 ? rand() % Aliens.size() : 0;
	Vector2 projectilePos = Aliens.at(randomAlienIndex).GetPosition();
	projectilePos.y += 40; // TODO: find what 40 represents
	AlienProjectiles.emplace_back(Projectile(projectilePos, false));
}

void Game::RenderGameplay() noexcept {
	background.Render();
	player.Render(resources.GetShip(renderer.GetPlayerActiveTexture()));
	std::ranges::for_each(PlayerProjectiles, [&](auto v) noexcept { v.Render(resources.GetProjectile()); });
	std::ranges::for_each(AlienProjectiles, [&](auto v) noexcept { v.Render(resources.GetProjectile()); });
	std::ranges::for_each(Walls, [&](auto v) noexcept { v.Render(resources.GetWall()); });
	std::ranges::for_each(Aliens, [&](auto v) noexcept { v.Render(resources.GetAlien()); });
	// TODO: Make this into a Renderer Func
	DrawText(TextFormat("Score: %i", leaderboard.GetScore()), 50, 20, 40, YELLOW);
	DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, 40, YELLOW);
}
void Game::UpdateBenchmark() noexcept{
	const unsigned __int64 now = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	frameCounter += 1;
	if ((now - frameStartTime) >= 1000) {
		frameStartTime = now;
		savedframeCounter = frameCounter;
		frameCounter = 0;
	}
}