#include "raylib.h"
#include "game.h"

int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "SPACE INVADERS");

    SetTargetFPS(60);

    Game game = { State::STARTSCREEN };
    Resources resources;
    game.resources = resources;
    game.Launch();

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