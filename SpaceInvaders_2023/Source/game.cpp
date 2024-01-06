#include "game.hpp"
#include "Helper.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>

void Game::Start() {
	SpawnWalls();
	player = Player();
	SpawnAliens();
	background = Background();
	score = 0;
	leaderboard = Leaderboard();
	gameState = State::GAMEPLAY;
}

void Game::End() noexcept {
	Projectiles.clear();
	Walls.clear();
	Aliens.clear();
	newHighScore = leaderboard.CheckNewHighScore(score);
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
		if (!newHighScore) {
			if (IsKeyReleased(KEY_ENTER)) { ShowStartScreen(); }
			break;
		}

		CheckCollisionPointRec(GetMousePosition(), textBox) ? MouseOnEndScreenText() : MouseNotOnEndScreenText();

		if (letterCount > 0 && letterCount < 9 && IsKeyReleased(KEY_ENTER)) {
			std::string nameEntry(name);
			leaderboard.InsertNewHighScore(nameEntry, score);
			newHighScore = false;
		}
		break;
	default:
		break;
	}
}

void Game::Render() {
	switch (gameState) {
	case State::STARTSCREEN:
		RenderStartScreen();
		break;
	case State::GAMEPLAY:
		RenderGameplay();
		break;
	case State::ENDSCREEN:
		newHighScore ? RenderEndScreenHighscore() : RenderEndScreen();
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
				Alien(aliensFormationX + 450 + (col * alienSpacing), aliensFormationY + (row * alienSpacing))
			);
		}
	}
}

bool Game::IsEndConditionTriggered() {
	bool hasAlienReached = false;
	if (Aliens.size() >= 1) {
		hasAlienReached = Aliens.at(0).HasReachedYPosition(static_cast<float>(GetScreenHeight()) - PLAYER_BASE_HEIGHT);
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
					score += scoreAddifyer;
				}
			}
		}

		else {
			if (CheckCollision({ player.GetPosition(), static_cast<float>(GetScreenHeight()) - PLAYER_BASE_HEIGHT }, PLAYER_RADIUS, projectile.lineStart, projectile.lineEnd)) {
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
	const Vector2 projectilePos = { player.GetPosition(), static_cast<float>(GetScreenHeight()) - 130 };
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

void Game::MouseOnEndScreenText() noexcept {
	mouseOnText = true;
	SetMouseCursor(MOUSE_CURSOR_IBEAM);

	int key = GetCharPressed();

	while (key > 0) {
		if ((key >= 32) && (key <= 125) && (letterCount < 9)) {
			name[letterCount] = (char)key;
			name[letterCount + 1] = '\0';
			letterCount++;
		}

		key = GetCharPressed();
	}

	if (IsKeyPressed(KEY_BACKSPACE)) {
		letterCount--;
		if (letterCount < 0) {
			letterCount = 0;
		}
		name[letterCount] = '\0';
	}
	framesCounter++;
}

void Game::MouseNotOnEndScreenText() noexcept {
	mouseOnText = false;
	SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	framesCounter = 0;
}

void Game::RenderStartScreen() noexcept {
	DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
	DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
}

void Game::RenderGameplay() {
	background.Render();
	DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
	DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, 40, YELLOW);

	player.Render(shipTextures.at(player.GetActiveTexture())._tex);

	for (auto& projectile : Projectiles) {
		projectile.Render(laserTexture._tex);
	}
	for (auto& wall : Walls) {
		wall.Render(barrierTexture._tex);
	}
	for (auto& alien : Aliens) {
		alien.Render(alienTexture._tex);
	}
}

void Game::RenderEndScreenHighscore() noexcept {
	DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);
	DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);

	DrawRectangleRec(textBox, LIGHTGRAY);
	if (mouseOnText) {
		DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
	}
	else {
		DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
	}
	DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

	DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 8), 600, 600, 20, YELLOW);

	if (mouseOnText) {
		if (letterCount < 9) {
			if (((framesCounter / 20) % 2) == 0) {
				DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
			}
		}
		else {
			DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
		}
	}
	if (letterCount > 0 && letterCount < 9) {
		DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
	}
}

void Game::RenderEndScreen() noexcept {
	DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);

	DrawText("LEADERBOARD", 50, 100, 40, YELLOW);

	for (int i = 0; i < leaderboard.list.size(); i++) {
		char* tempNameDisplay = leaderboard.list[i].name.data();
		DrawText(tempNameDisplay, 50, 140 + (i * 40), 40, YELLOW);
		DrawText(TextFormat("%i", leaderboard.list[i].score), 350, 140 + (i * 40), 40, YELLOW);
	}
}