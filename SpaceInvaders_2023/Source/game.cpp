#include "game.hpp"
#include "Helper.hpp"
#include <vector>
#include <algorithm>
// TODO: move class functions and variables around so they follow early exits
// TODO: attempt to const all renderers, check all functions const validity
// TODO: remove ALL magic constants in Renderer, figure out what they represent if any
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
		if (IsEndGameConditionTriggered()) {
			End();
			break;
		}
		renderer.UpdatePlayerAnimation();
		player.Update();
		for (auto& alien : Aliens) { alien.Update(); }
		std::ranges::for_each(PlayerProjectiles, [&](auto &v) noexcept { v.Update(); });
		std::ranges::for_each(AlienProjectiles, [&](auto &v) noexcept { v.Update(); });

		HandleProjectileHit();
		if (IsKeyPressed(KEY_SPACE)) { PlayerShoot(); }
		if (Aliens.size() < 1) { SpawnAliens(); }
		AliensShoot();
		break;
	case State::ENDSCREEN:
		if (!leaderboard.IsNewHighScore()) {
			if (IsKeyReleased(KEY_ENTER)) { ShowStartScreen(); }
			break;
		}
		leaderboard.UpdateHighscoreName(renderer.mouseOnText());
		if (IsKeyReleased(KEY_ENTER) && leaderboard.IsHighscoreNameValid()) {
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
			renderer.HighscoreScreen(leaderboard.GetTempHighscoreName(), MAX_INPUT_CHARS);
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
		const auto pos = Vector2{wallDistance * (i+1), wallsPositionY};
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

bool Game::IsEndGameConditionTriggered() noexcept {
	bool hasAlienReachedWalls = false;
	if (Aliens.size() > 0) {
#pragma warning(push) // using size()-1 is a good way to ensure we don't check beyond
#pragma warning(disable:26446)
		hasAlienReachedWalls = Aliens[Aliens.size() - 1].HasReachedYPosition(player.GetPositionY());
#pragma warning(pop)
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
void Game::RemoveOutOfScreenProjectiles(std::vector<Projectile> &v) noexcept {
    v.erase(std::remove_if(v.begin(), v.end(), [](const auto& projectile) noexcept {
        return projectile.IsOutOfScreen();
    }), v.end());
}
void Game::RemoveWallHitProjectiles(std::vector<Projectile> &v) noexcept {
    v.erase(std::remove_if(v.begin(), v.end(), [&](const auto& projectile) noexcept {
        return IsWallHit(projectile.GetPosition());
    }), v.end());
}
void Game::RemoveAlienHitProjectiles(std::vector<Projectile> &v) noexcept {
    v.erase(std::remove_if(v.begin(), v.end(), [&](const auto& projectile) noexcept {
        return IsAlienHit(projectile.GetPosition());
    }), v.end());
}
void Game::RemovePlayerHitProjectiles(std::vector<Projectile> &v) noexcept {
    v.erase(std::remove_if(v.begin(), v.end(), [&](const auto& projectile) noexcept {
        return IsPlayerHit(projectile.GetPosition());
    }), v.end());
}
bool Game::IsAlienHit(Vector2 projectilePosition) noexcept { // TODO: maybe find a better name...
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
	alienShootTimer += 1;
	if (alienShootTimer < SHOOT_DELAY) { return; }
	alienShootTimer = 0;

	const int randomAlienIndex = Aliens.size() > 1 ? rand() % Aliens.size() : 0;
#pragma warning(push) // randomAlienIndex takes 0 if there's only one alien, and there will always be an alien due to SpawnAlien()
#pragma warning(disable:26446)
	Vector2 projectilePos = Aliens[randomAlienIndex].GetPosition();
#pragma warning(pop)
	projectilePos.y += 40; // TODO: find what 40 represents
	AlienProjectiles.emplace_back(Projectile(projectilePos, false));
}

void Game::RenderGameplay() const noexcept {
	background.Render(player.GetPosition(), {0,PLAYER_BASE_HEIGHT});
	player.Render(resources.GetShip(renderer.GetPlayerActiveTexture()));
	std::ranges::for_each(PlayerProjectiles, [&](auto v) noexcept { v.Render(resources.GetProjectile()); });
	std::ranges::for_each(AlienProjectiles, [&](auto v) noexcept { v.Render(resources.GetProjectile()); });
	std::ranges::for_each(Walls, [&](auto v) noexcept { v.Render(resources.GetWall()); });
	std::ranges::for_each(Aliens, [&](auto v) noexcept { v.Render(resources.GetAlien()); });
	renderer.GameplayText(leaderboard.GetScore(), player.GetLives());
}