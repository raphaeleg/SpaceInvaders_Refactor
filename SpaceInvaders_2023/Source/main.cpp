#include "raylib.h"
#include "game.h"

int main(void)
{
    constexpr float screenWidth = 1920.0f;
    constexpr float screenHeight = 1080.0f;

    InitWindow(screenWidth, screenHeight, "SPACE INVADERS");

    SetTargetFPS(60);

    Game game = { State::STARTSCREEN };

    while (!WindowShouldClose())
    {
        game.Update();
        BeginDrawing();
        ClearBackground(BLACK);
        game.Render();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}