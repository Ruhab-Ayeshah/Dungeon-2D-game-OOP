#include "Map.h"
#include "Player.h"
#include "Golem.h"
#include <raylib.h>
#include <raymath.h>

Golem *globalGolem = nullptr; // define here

int main()
{
    const int virtualWidth = 960;
    const int virtualHeight = 640;
    const int tileSize = 32;
    const int mapCols = 30;
    const int mapRows = 20;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_FULLSCREEN_MODE);
    InitWindow(virtualWidth, virtualHeight, "2D Dungeon Explorer");

    ToggleFullscreen();
    SetTargetFPS(60);

    Map Level1("assets/Map_Assets/Level1.txt");
    Player PlayerTest(Level1.getSpawn());
    Golem golem({200, 200});
    globalGolem = &golem;

    golem.SetTarget(&PlayerTest.Position);
    golem.SetMap(&Level1);

    Camera2D camera = {0};
    camera.zoom = 3.0f;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        PlayerTest.HandleInput(&Level1);
        PlayerTest.Update(dt);
        golem.Update(dt);

        Rectangle playerHitbox = {PlayerTest.Position.x, PlayerTest.Position.y, 32, 32};
        Rectangle golemHitbox = {golem.Position.x, golem.Position.y, 64, 64};

        if (IsKeyPressed(KEY_SPACE) && !golem.IsDead())
        {
            if (CheckCollisionRecs(playerHitbox, golemHitbox))
            {
                golem.TakeDamage(1);
            }
        }

        if (golem.attackDone && !PlayerTest.IsDead())
        {
            if (CheckCollisionRecs(playerHitbox, golemHitbox))
            {
                PlayerTest.TakeDamage(1);
                golem.attackDone = false;
            }
        }

        Vector2 playerCenter = {
            PlayerTest.Position.x + tileSize / 2,
            PlayerTest.Position.y + tileSize / 2};

        float halfWidth = (float)GetScreenWidth() / (2.0f * camera.zoom);
        float halfHeight = (float)GetScreenHeight() / (2.0f * camera.zoom);

        camera.target.x = Clamp(playerCenter.x, halfWidth, mapCols * tileSize - halfWidth);
        camera.target.y = Clamp(playerCenter.y, halfHeight, mapRows * tileSize - halfHeight);
        camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        Level1.Draw();
        PlayerTest.Draw();
        golem.Draw();
        EndMode2D();

        if (PlayerTest.IsDead())
        {
            DrawText("You Died!", 400, 300, 40, RED);
        }
        if (golem.IsDead())
        {
            DrawText("Golem Died!", 400, 340, 40, GREEN);
        }

        DrawText("Move: WASD | Attack: SPACE | Exit: ESC", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
