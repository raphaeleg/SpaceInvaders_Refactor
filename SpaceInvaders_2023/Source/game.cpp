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
	Projectiles.clear();
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
		background.Update(player.GetPosition());

		HandleProjectileHit();
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
		if (IsKeyReleased(KEY_ENTER) && draftHighscoreName.size() > 0 && draftHighscoreName.size() < MAX_INPUT_CHARS) {
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
		renderer.StartScreen();
		break;
	case State::GAMEPLAY:
		RenderGameplay();
		break;
	case State::ENDSCREEN:
		if (leaderboard.IsNewHighScore()) {
			renderer.HighscoreScreen(draftHighscoreName);
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
		hasAlienReachedWalls = Aliens.at(Aliens.size() - 1).HasReachedYPosition(GetScreenHeight() - static_cast<int>(PLAYER_BASE_HEIGHT));
	}
	return IsKeyReleased(KEY_Q) || player.IsDead() || hasAlienReachedWalls;
}

void Game::HandleProjectileHit() noexcept {
	for (auto& projectile : Projectiles) {
		projectile.Update();
		const Vector2 projPos = projectile.GetPosition();
		if (HandledWallHit(projPos)) {
			projectile.Hit();
			continue;
		};
		if (projectile.IsPlayerProjectile()) {
			if (HandledAlienHit(projPos)) { projectile.Hit(); }
			continue;
		}
		if (HandledPlayerHit(projPos)) { projectile.Hit(); }
	}
}
bool Game::HandledAlienHit(Vector2 projectilePosition) noexcept {
	const auto findAlienHit = std::ranges::find_if(Aliens, [&](auto alien) noexcept {
		return CheckCollision(alien.GetPosition(), ALIEN_RADIUS, projectilePosition);
		});
	if (findAlienHit == Aliens.end()) { return false; }
	Aliens.at(std::distance(Aliens.begin(), findAlienHit)).Kill();
	leaderboard.AddScore();
	return true;
}
bool Game::HandledPlayerHit(Vector2 projectilePosition) noexcept {
	const Vector2 calcPlayerPos = { player.GetPosition(), GetScreenHeightF() - PLAYER_BASE_HEIGHT };
	if (!CheckCollision(calcPlayerPos, PLAYER_RADIUS, projectilePosition)) { return false; }
	player.DecreaseHealth();
	return true;
}
bool Game::HandledWallHit(Vector2 projectilePosition) noexcept {
	const auto findWallHit = std::ranges::find_if(Walls, [&](auto wall) noexcept {
		return CheckCollision(wall.GetPosition(), WALL_RADIUS, projectilePosition);
		});
	if (findWallHit == Walls.end()) { return false; }
	Walls.at(std::distance(Walls.begin(), findWallHit)).DecreaseHealth();
	return true;
}

void Game::PlayerShoot() {
	const Vector2 projectilePos = { player.GetPosition(), GetScreenHeightF() - 130 };
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
	player.Render(resources.GetShip(renderer.GetPlayerActiveTexture()));
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
void Game::UpdateBenchmark() noexcept{
	const unsigned __int64 now = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	frameCounter += 1;
	if ((now - frameStartTime) >= 1000) {
		frameStartTime = now;
		savedframeCounter = frameCounter;
		frameCounter = 0;
	}
}