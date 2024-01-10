#include "raylib.h"
#include "game.hpp"
#include "Window.hpp"

int main(void) {
	Window window{ 1920, 1080, "SPACE INVADERS" };
	SetTargetFPS(60);
	Game game = Game();

	while (!WindowShouldClose()) {
		game.Update();
		BeginDrawing();
		ClearBackground(BLACK);
		game.Render();
		EndDrawing();
	}
	return 0;
}