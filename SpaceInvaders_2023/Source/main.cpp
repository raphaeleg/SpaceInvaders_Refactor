#include "raylib.h"
#include "game.hpp"
#include "Helper.hpp"
//TODO: remove all const members. 
//TODO: InitWindow / CloseWindow looks very much like a resource... 
//TODO: a Window object could probably also configure the FTP
//TODO: catch the exceptions that you are throwing. Log them. 
//TODO: get rid of drawtexturepro and all the shitty rectangles that nobody needs. no reason to scale sprites every frame, resize the png
//TODO: simplify star / background. pass in player pos, apply offset at (const) star::render. 

int main(void) {
	InitWindow(SCREEN_WIDTH_INT, SCREEN_HEIGHT_INT, "SPACE INVADERS");
	SetTargetFPS(60);
	Game game{};

	while (!WindowShouldClose()) {
		game.Update();
		BeginDrawing();
		ClearBackground(BLACK);
		game.Render();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}