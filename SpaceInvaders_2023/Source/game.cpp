#include "game.h"
#include "Helper.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>

void Game::Start()
{
	float window_width = (float)GetScreenWidth();
	float window_height = (float)GetScreenHeight();
	float wall_distance = window_width / (wallCount + 1);
	for (int i = 0; i < wallCount; i++)
	{
		Wall newWalls;
		newWalls.position.y = window_height - 250;
		newWalls.position.x = wall_distance * (i + 1);

		Walls.push_back(newWalls);
	}

	Player newPlayer;
	player = newPlayer;
	player.Initialize();

	SpawnAliens();


	Background newBackground;
	newBackground.Initialize(600);
	background = newBackground;

	score = 0;

	gameState = State::GAMEPLAY;
}

void Game::End()
{
	Projectiles.clear();
	Walls.clear();
	Aliens.clear();
	newHighScore = CheckNewHighScore();
	gameState = State::ENDSCREEN;
}

void Game::Continue()
{
	gameState = State::STARTSCREEN;
}

void Game::Launch()
{
	resources.Load();
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

			if (alien.position.y > GetScreenHeight() - player.player_base_height)
			{
				End();
			}
		}

		if (player.lives < 1)
		{
			End();
		}

		if (Aliens.size() < 1)
		{
			SpawnAliens();
		}

		playerPos = { player.x_pos, (float)player.player_base_height };
		cornerPos = { 0, (float)player.player_base_height };
		offset = lineLength(playerPos, cornerPos) * -1;
		background.Update(offset / 15);


		for (auto& projectile : Projectiles)
		{
			projectile.Update();
		}
		for (auto& wall : Walls)
		{
			wall.Update();
		}

		for (auto& projectile : Projectiles)
		{
			if (projectile.type == EntityType::PLAYER_PROJECTILE)
			{
				for (auto& alien : Aliens)
				{
					if (CheckCollision(alien.position, alien.radius, projectile.lineStart, projectile.lineEnd))
					{
						projectile.active = false;
						alien.active = false;
						score += 100;
					}
				}
			}

			if (projectile.type == EntityType::ENEMY_PROJECTILE)
			{
				if (CheckCollision({ player.x_pos, GetScreenHeight() - player.player_base_height }, player.radius, projectile.lineStart, projectile.lineEnd))
				{
					projectile.active = false;
					player.lives -= 1;
				}
			}

			for (auto& wall : Walls)
			{
				if (CheckCollision(wall.position, wall.radius, projectile.lineStart, projectile.lineEnd))
				{
					projectile.active = false;
					wall.health -= 1;
				}
			}
		}

		if (IsKeyPressed(KEY_SPACE))
		{
			float window_height = (float)GetScreenHeight();
			Projectile newProjectile;
			newProjectile.position.x = player.x_pos;
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
			newProjectile.position = Aliens.at(randomAlienIndex).position;
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
			if (Aliens.at(i).active == false)
			{
				Aliens.erase(Aliens.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < Walls.size(); i++)
		{
			if (Walls.at(i).active == false)
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
						name[letterCount+1] = '\0';
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

				InsertNewHighScore(nameEntry);

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
		DrawText(TextFormat("Lives: %i", player.lives), 50, 70, 40, YELLOW);

		player.Render(resources.shipTextures[player.activeTexture]);

		for (auto& projectile : Projectiles)
		{
			projectile.Render(resources.laserTexture);
		}

		for (auto& wall : Walls)
		{
			wall.Render(resources.barrierTexture);
		}

		for (auto& alien : Aliens)
		{
			alien.Render(resources.alienTexture);
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

			for (int i = 0; i < Leaderboard.size(); i++)
			{
				char* tempNameDisplay = Leaderboard.at(i).name.data();
				DrawText(tempNameDisplay, 50, 140 + (i * 40), 40, YELLOW);
				DrawText(TextFormat("%i", Leaderboard.at(i).score), 350, 140 + (i * 40), 40, YELLOW);
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
			Alien newAlien = Alien();
			newAlien.active = true;
			newAlien.position.x = formationX + 450 + (col * alienSpacing);
			newAlien.position.y = formationY + (row * alienSpacing);
			Aliens.push_back(newAlien);
		}
	}

}

bool Game::CheckNewHighScore()
{
	if (score > Leaderboard.at(4).score)
	{
		return true;
	}
	return false;
}

void Game::InsertNewHighScore(std::string name)
{
	PlayerData newData;
	newData.name = name;
	newData.score = score;

	for (int i = 0; i < Leaderboard.size(); i++)
	{
		if (newData.score > Leaderboard.at(i).score)
		{
			Leaderboard.insert(Leaderboard.begin() + i, newData);

			Leaderboard.pop_back();

			i = Leaderboard.size();
		}
	}
}

void Player::Initialize()
{
	float window_width = (float)GetScreenWidth();
	x_pos = window_width / 2;
}

void Player::Update()
{
	direction = 0;
	if (IsKeyDown(KEY_LEFT))
	{
		direction--;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		direction++;
	}

	x_pos += speed * direction;

	if (x_pos < 0 + radius)
	{
		x_pos = 0 + radius;
	}
	else if (x_pos > GetScreenWidth() - radius)
	{
		x_pos = GetScreenWidth() - radius;
	}

	timer += GetFrameTime();

	if (timer > 0.4 && activeTexture == 2)
	{
		activeTexture = 0;
		timer = 0;
	}
	else if (timer > 0.4)
	{
		activeTexture++;
		timer = 0;
	}
}

void Player::Render(Texture2D texture)
{
	float window_height = GetScreenHeight();
	DrawTexturePro(texture,{0,0,352,352},{x_pos, window_height - player_base_height,100,100},{50,50},0,WHITE);
}

void Projectile::Update()
{
	position.y -= speed;

	lineStart.y = position.y - 15;
	lineEnd.y = position.y + 15;

	lineStart.x = position.x;
	lineEnd.x = position.x;

	if (position.y < 0 || position.y > 1500)
	{
		active = false;
	}
}

void Projectile::Render(Texture2D texture)
{
	DrawTexturePro(texture,{0,0,176,176},{position.x,position.y,50,50}, {25,25},0,WHITE);
}

void Wall::Render(Texture2D texture)
{
	DrawTexturePro(texture,{0,0,704,704},{position.x,position.y,200,200},{100,100},0,WHITE);
	DrawText(TextFormat("%i", health), position.x - 21, position.y + 10, 40, RED);
}

void Wall::Update()
{
	if (health < 1)
	{
		active = false;
	}
}

void Alien::Update()
{
	int window_width = GetScreenWidth();

	if (moveRight)
	{
		position.x += speed;

		if (position.x >= GetScreenWidth())
		{
			moveRight = false;
			position.y += 50;
		}
	}
	else
	{
		position.x -= speed;

		if (position.x <= 0)
		{
			moveRight = true;
			position.y += 50;
		}
	}
}

void Alien::Render(Texture2D texture)
{
	DrawTexturePro(texture,{0,0,352,352},{position.x,position.y,100,100},{50,50},0,WHITE);
}

void Star::Update(float starOffset)
{
	position.x = initPosition.x + starOffset;
	position.y = initPosition.y;
}

void Star::Render()
{
	DrawCircle((int)position.x, (int)position.y, size, color);
}


void Background::Initialize(int starAmount)
{
	for (int i = 0; i < starAmount; i++)
	{
		Star newStar;

		newStar.initPosition.x = GetRandomValue(-150, GetScreenWidth() + 150);
		newStar.initPosition.y = GetRandomValue(0, GetScreenHeight());

		newStar.color = SKYBLUE;

		newStar.size = GetRandomValue(1, 4) / 2;

		Stars.push_back(newStar);

	}
}

void Background::Update(float offset)
{
	for (auto& star : Stars)
	{
		star.Update(offset);
	}
}

void Background::Render()
{
	for (auto& star : Stars)
	{
		star.Render();
	}
}