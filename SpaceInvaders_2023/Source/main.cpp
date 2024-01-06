#include "raylib.h"
#include "game.hpp"
#include "Helper.hpp"

int main(void) {
    InitWindow(SCREEN_WIDTH_INT, SCREEN_HEIGHT_INT, "SPACE INVADERS");
    SetTargetFPS(60);
    Game game = Game();

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