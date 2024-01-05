#include "game.h"
#include "Helper.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>

void Game::Start()
{
	for (int i = 1; i < wallCount + 1; i++)
	{
		Walls.push_back(Wall(wall_distance * i, wallsY));
	}

	player = Player();

	SpawnAliens();

	background = Background();

	score = 0;

	leaderboard = Leaderboard();

	gameState = State::GAMEPLAY;
}

void Game::End()
{
	Projectiles.clear();
	Walls.clear();
	Aliens.clear();
	newHighScore = leaderboard.CheckNewHighScore(score);
	gameState = State::ENDSCREEN;
}

void Game::Continue()
{
	gameState = State::STARTSCREEN;
}

void Game::Update()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		if (IsKeyReleased(KEY_SPACE))
		{
			Start();
		}
		break;

	case State::GAMEPLAY:
		if (IsKeyReleased(KEY_Q))
		{
			End();
		}

		player.Update();

		for (auto& alien : Aliens)
		{
			alien.Update();

			if (alien.HasReachedYPosition(GetScreenHeight() - PLAYER_BASE_HEIGHT))
			{
				End();
			}
		}

		if (player.IsDead())
		{
			End();
		}

		if (Aliens.size() < 1)
		{
			SpawnAliens();
		}

		playerPos = { player.GetPosition(), PLAYER_BASE_HEIGHT };
		cornerPos = { 0, PLAYER_BASE_HEIGHT };
		offset = lineLength(playerPos, cornerPos) * -1;
		background.Update(offset / 15);

		for (auto& projectile : Projectiles)
		{
			projectile.Update();
		}

		for (auto& projectile : Projectiles)
		{
			if (projectile.type == EntityType::PLAYER_PROJECTILE)
			{
				for (auto& alien : Aliens)
				{
					if (CheckCollision(alien.GetPosition(), ALIEN_RADIUS, projectile.lineStart, projectile.lineEnd))
					{
						projectile.active = false;
						alien.Kill();
						score += 100;
					}
				}
			}

			if (projectile.type == EntityType::ENEMY_PROJECTILE)
			{
				if (CheckCollision({ player.GetPosition(), GetScreenHeight() - PLAYER_BASE_HEIGHT }, PLAYER_RADIUS, projectile.lineStart, projectile.lineEnd))
				{
					projectile.active = false;
					player.DecreaseHealth();
				}
			}

			for (auto& wall : Walls)
			{
				if (CheckCollision(wall.GetPosition(), WALL_RADIUS, projectile.lineStart, projectile.lineEnd))
				{
					projectile.active = false;
					wall.DecreaseHealth();
				}
			}
		}

		if (IsKeyPressed(KEY_SPACE))
		{
			float window_height = (float)GetScreenHeight();
			Projectile newProjectile;
			newProjectile.position.x = player.GetPosition();
			newProjectile.position.y = window_height - 130;
			newProjectile.type = EntityType::PLAYER_PROJECTILE;
			Projectiles.push_back(newProjectile);
		}

		shootTimer += 1;
		if (shootTimer > 59)
		{
			int randomAlienIndex = 0;

			if (Aliens.size() > 1)
			{
				randomAlienIndex = rand() % Aliens.size();
			}

			Projectile newProjectile;
			newProjectile.position = Aliens.at(randomAlienIndex).GetPosition();
			newProjectile.position.y += 40;
			newProjectile.speed = -15;
			newProjectile.type = EntityType::ENEMY_PROJECTILE;
			Projectiles.push_back(newProjectile);
			shootTimer = 0;
		}

		for (int i = 0; i < Projectiles.size(); i++)
		{
			if (Projectiles.at(i).active == false)
			{
				Projectiles.erase(Projectiles.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < Aliens.size(); i++)
		{
			if (Aliens.at(i).IsDead())
			{
				Aliens.erase(Aliens.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < Walls.size(); i++)
		{
			if (Walls.at(i).IsDead())
			{
				Walls.erase(Walls.begin() + i);
				i--;
			}
		}
		break;

	case State::ENDSCREEN:
		if (IsKeyReleased(KEY_ENTER) && !newHighScore)
		{
			Continue();
		}

		if (newHighScore)
		{
			if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
			else mouseOnText = false;

			if (mouseOnText)
			{
				SetMouseCursor(MOUSE_CURSOR_IBEAM);

				int key = GetCharPressed();

				while (key > 0)
				{
					if ((key >= 32) && (key <= 125) && (letterCount < 9))
					{
						name[letterCount] = (char)key;
						name[letterCount + 1] = '\0';
						letterCount++;
					}

					key = GetCharPressed();
				}

				if (IsKeyPressed(KEY_BACKSPACE))
				{
					letterCount--;
					if (letterCount < 0) letterCount = 0;
					name[letterCount] = '\0';
				}
			}
			else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			if (mouseOnText)
			{
				framesCounter++;
			}
			else
			{
				framesCounter = 0;
			}

			if (letterCount > 0 && letterCount < 9 && IsKeyReleased(KEY_ENTER))
			{
				std::string nameEntry(name);

				leaderboard.InsertNewHighScore(nameEntry, score);

				newHighScore = false;
			}
		}
		break;
	default:
		break;
		}
	}

void Game::Render()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
		DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
		break;
	case State::GAMEPLAY:
		background.Render();
		DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
		DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, 40, YELLOW);

		player.Render(shipTextures.at(player.GetActiveTexture())._tex);

		for (auto& projectile : Projectiles)
		{
			projectile.Render(laserTexture._tex);
		}

		for (auto& wall : Walls)
		{
			wall.Render(barrierTexture._tex);
		}

		for (auto& alien : Aliens)
		{
			alien.Render(alienTexture._tex);
		}
		break;
	case State::ENDSCREEN:
		if (newHighScore)
		{
			DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);
			DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);

			DrawRectangleRec(textBox, LIGHTGRAY);
			if (mouseOnText)
			{
				DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
			}
			else
			{
				DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
			}
			DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

			DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 8), 600, 600, 20, YELLOW);

			if (mouseOnText)
			{
				if (letterCount < 9)
				{
					if (((framesCounter / 20) % 2) == 0)
					{
						DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
					}
				}
				else
				{
					DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
				}

			}
			if (letterCount > 0 && letterCount < 9)
			{
				DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
			}

		}
		else {
			DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);

			DrawText("LEADERBOARD", 50, 100, 40, YELLOW);

			for (int i = 0; i < leaderboard.list.size(); i++)
			{
				char* tempNameDisplay = leaderboard.list[i].name.data();
				DrawText(tempNameDisplay, 50, 140 + (i * 40), 40, YELLOW);
				DrawText(TextFormat("%i", leaderboard.list[i].score), 350, 140 + (i * 40), 40, YELLOW);
			}
		}
		break;
	default:
		break;
	}
}

void Game::SpawnAliens()
{
	for (int row = 0; row < formationHeight; row++) {
		for (int col = 0; col < formationWidth; col++) {
			Aliens.push_back(Alien(formationX + 450 + (col * alienSpacing), formationY + (row * alienSpacing)));
		}
	}
}