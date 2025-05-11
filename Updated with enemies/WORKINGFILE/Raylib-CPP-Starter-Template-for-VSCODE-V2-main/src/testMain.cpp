#include "Map.h"
#include "Player.h"
#include "Golem.h"
#include <raylib.h>
#include <raymath.h>
#include <string>

Golem *globalGolem = nullptr; // define here

float floatingTextTimer = 0.0f;
string floatingText = "";
Vector2 floatingTextPos = {0, 0};

int main()
{

///////////////////////////////// INITIAL SET UP STARTS ///////////////////////////////////////////////////////

    const int virtualWidth = 960;
    const int virtualHeight = 640;
    const int tileSize = 32;
    const int mapCols = 30;
    const int mapRows = 20;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_FULLSCREEN_MODE);
    InitWindow(virtualWidth, virtualHeight, "2D Dungeon Explorer");

    ToggleFullscreen();
    SetTargetFPS(60);

    Map Levels[2];

    Map Level1("assets/Map_Assets/Level1.txt",4,9,34,"assets/Map_Assets/Dungeon_Bricks_Shadow.png");
    Map Level2("assets/Map_Assets/Level2.txt",46,24,34,"assets/Map_Assets/Full.png");
    
    Levels[0] = Level1;
    Levels[1] = Level2;
    
    int currLevel = 0; 

    Player PlayerTest(Levels[currLevel].getSpawn());
    Golem golem({200, 200});
    globalGolem = &golem;

    golem.SetTarget(&PlayerTest.Position);
    golem.SetMap(&Levels[currLevel]);

    Camera2D camera = {0};
    camera.zoom = 3.0f;
///////////////////////////////// INITIAL SET UP ENDS ///////////////////////////////////////////////////////



    while (!WindowShouldClose())
    {
        
        float dt = GetFrameTime(); /// FOR ANIMATIONS

/////////////////////////////////// CAMERA LOGIC ///////////////////////////////////

        Vector2 playerCenter = {PlayerTest.Position.x + tileSize / 2, PlayerTest.Position.y + tileSize / 2};

        float halfWidth = (float)GetScreenWidth() / (2.0f * camera.zoom);
        float halfHeight = (float)GetScreenHeight() / (2.0f * camera.zoom);

        camera.target.x = Clamp(playerCenter.x, halfWidth, mapCols * tileSize - halfWidth);
        camera.target.y = Clamp(playerCenter.y, halfHeight, mapRows * tileSize - halfHeight);
        camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

/////////////////////////////////// CAMERA LOGIC ///////////////////////////////////

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

/////////////////////////////////// DRAWING ///////////////////////////////////
        Levels[currLevel].Draw(); 
        PlayerTest.Draw();
        golem.Draw();


        PlayerTest.HandleInput(&Levels[currLevel]);
        PlayerTest.Update(dt);
        golem.Update(dt);


/////////////////////////////////// GOLEM AND PLAYER INTERACTION /////////////////////////////////////////

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

        if (PlayerTest.IsDead())
        {
            DrawText("You Died!", 400, 300, 40, RED);
        }
        if (golem.IsDead())
        {
            DrawText("Golem Died!", 400, 340, 40, GREEN);
        }
/////////////////////////////////// GOLEM AND PLAYER INTERACTION /////////////////////////////////////////


/////////////////////////////////// CHECK IF PLAYER AT EXIT /////////////////////////////////////////

        if(currLevel<1 &&Vector2Equals(PlayerTest.getGridPos(), Levels[currLevel].getExit())){
            currLevel++;
            PlayerTest.ResetToSpawn(Levels[currLevel].getSpawn());
            golem.SetTarget(&PlayerTest.Position);
            golem.SetMap(&Levels[currLevel]);
        }
/////////////////////////////////// CHECK IF PLAYER AT EXIT /////////////////////////////////////////


/////////////////////////////////// COLLECTING ITEMS LOGIC /////////////////////////////////////////
        Vector2 tilePos = {PlayerTest.Position.x / 32,PlayerTest.Position.y / 32};

        if (Levels[currLevel].getMap()[(int)tilePos.y][(int)tilePos.x].collectable) ///// IS CURRENT TILE A COLLECTABLE?
        {
            Collectable* c = Levels[currLevel].getcollectables()[(int)tilePos.y][(int)tilePos.x]; 
            if (!c->IsCollected())
            {
                c->Collect(); 

                /////////////////////////// HEALTH COLLECTABLE ////////////////////////////////

                if(c->getType()=="Health"){
                    PlayerTest.setHealth(c->getValue());
                    floatingText = "+10 Health";
                    floatingTextPos = {PlayerTest.Position.x, PlayerTest.Position.y - 10};
                    floatingTextTimer = 2.0f; // Show for 2 seconds

                }
                
                /////////////////////////// SCORE COLLECTABLE ////////////////////////////////s
                
                else{
                    PlayerTest.setScore(c->getValue());
                    floatingText = "+10 Score";
                    floatingTextPos = {PlayerTest.Position.x, PlayerTest.Position.y - 10};
                    floatingTextTimer = 2.0f; // Show for 2 seconds



                }
            }
        }

        ////////////////////////// TEXT DISPLAYING AMOUNT OF SCORE/HEALTH GAINED ///////////////////////////

        if (floatingTextTimer > 0.0f) {
        floatingTextTimer -= GetFrameTime();
        DrawText(floatingText.c_str(), (int)floatingTextPos.x, (int)floatingTextPos.y, 14, GREEN);
        }


        DrawText("Move: WASD | Attack: SPACE | Exit: ESC", 10, 10, 20, RED);

        EndMode2D();

        

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
