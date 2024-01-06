#include "raylib.h"
#include "game.hpp"

constexpr float SCREEN_HEIGHT = 1080.0f;
constexpr float SCREEN_WIDTH = 1920.0f;

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SPACE INVADERS");

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