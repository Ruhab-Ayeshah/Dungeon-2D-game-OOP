#include "Map.h"
#include "Player.h"
#include <raylib.h>
#include <raymath.h>

int main() {
    const int virtualWidth = 960;
    const int virtualHeight = 640;
    const int tileSize = 32;
    const int mapCols = 30;
    const int mapRows = 20;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_FULLSCREEN_MODE);
    InitWindow(virtualWidth, virtualHeight, "2D Dungeon Explorer");

    // Switch to full screen using the current monitor resolution
    ToggleFullscreen();

    SetTargetFPS(60);

    Map Level1("assets/Map_Assets/Level1.txt");
    Player PlayerTest(Level1.getSpawn());

    Camera2D camera = {0};

    // Zoom: display only a small area (~9 tiles)
    camera.zoom = 3.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        PlayerTest.HandleInput(&Level1);
        PlayerTest.Update(dt);
        PlayerTest.Update_Animation(PlayerTest.getCurrentAnim());

        Vector2 playerCenter = {
            PlayerTest.Position.x + tileSize / 2,
            PlayerTest.Position.y + tileSize / 2
        };

        float halfWidth = (float)GetScreenWidth() / (2.0f * camera.zoom);
        float halfHeight = (float)GetScreenHeight() / (2.0f * camera.zoom);

        camera.target.x = Clamp(playerCenter.x, halfWidth, mapCols * tileSize - halfWidth);
        camera.target.y = Clamp(playerCenter.y, halfHeight, mapRows * tileSize - halfHeight);
        camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

        


        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        Level1.Draw();
        PlayerTest.Draw();
        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
