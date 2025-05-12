#include "Map.h"
#include "Player.h"
#include "button.h"
#include "Golem.h"
#include "TextManager.h"
#include <raylib.h>
#include <raymath.h>
#include <string>

Golem *globalGolem = nullptr; // define here

float floatingTextTimer = 0.0f;
string floatingText = "";
Vector2 floatingTextPos = {0, 0};

bool gameStarted = false;
bool gameOver = false;

int main()
{

///////////////////////////////// INITIAL SET UP STARTS ///////////////////////////////////////////////////////

    const int virtualWidth = GetScreenWidth();
    const int virtualHeight = GetScreenHeight();
    const int tileSize = 32;
    const int mapCols = 30;
    const int mapRows = 20;

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(virtualWidth, virtualHeight, "2D Dungeon Explorer");
    
    ToggleFullscreen();
    SetTargetFPS(60);

    Texture2D background = LoadTexture("assets/menu.png");  
    Texture2D exitbackground = LoadTexture("assets/exit.png");
    
    bool gDeath = false;

    Map Levels[2];

    Vector2 screenCenter = {(GetScreenWidth() - 1024 * 0.5f) / 2,(GetScreenHeight() - 1024 * 0.5f) / 2};

    Button button("assets/button.png", screenCenter);
    Texture2D congrats = LoadTexture("assets/escapeText.png");
    Texture2D exitText = LoadTexture("assets/exitText.png");

    Map Level1("assets/Map_Assets/Level1.txt",4,9,34,"assets/Map_Assets/Dungeon_Bricks_Shadow.png");
    Map Level2("assets/Map_Assets/Level2.txt",46,24,34,"assets/Map_Assets/Full.png");
    
    Levels[0] = Level1;
    Levels[1] = Level2;
    
    int currLevel = 0; 

    Player PlayerTest(Levels[currLevel].getSpawn());
    Golem golem({250, 250});
    globalGolem = &golem;

    golem.SetTarget(&PlayerTest.Position);
    golem.SetMap(&Levels[currLevel]);

    TextManager Text;

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
        if(!gameStarted){

            DrawTexturePro(background,{0,0,(float)(int)background.width,(float)(int)background.height},{0,0,(float)(int)GetScreenWidth(),(float)(int)GetScreenHeight()},{0.0f,0.0f},0.0f,WHITE);
           
            button.Draw();
           
            if(button.isClicked()){
                gameStarted = true;
            }


        }else if(gameOver){

            
            DrawTexturePro(exitbackground,{0,0,(float)(int)exitbackground.width,(float)(int)exitbackground.height},{0,0,(float)(int)GetScreenWidth(),(float)(int)GetScreenHeight()},{0.0f,0.0f},0.0f,WHITE);
            
            float scale = 0.5f;
            float scaledW = congrats.width * scale;
            float scaledH = congrats.height * scale;
            Vector2 center = { (GetScreenWidth() - scaledW) / 2.0f, (GetScreenHeight() - scaledH) / 2.0f };

            DrawTexturePro(congrats,{0, 0, (float)congrats.width, (float)congrats.height},{center.x, center.y, scaledW, scaledH}, {0.0f, 0.0f}, 0.0f, WHITE);

           DrawTexturePro(exitText,{0, 0, (float)exitText.width, (float)exitText.height},{center.x, (GetScreenHeight()+center.y)/2, (float)(int)exitText.width*scale, (float)(int)exitText.height*scale}, {0.0f, 0.0f}, 0.0f, WHITE);
        
            if(IsKeyPressed(KEY_ESCAPE)){
                break;
            }


        }else{
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
        Rectangle golemHitbox = {golem.Position.x, golem.Position.y, 32, 32};

        if (IsKeyPressed(KEY_SPACE) && !golem.IsDead())
        {
            if (CheckCollisionRecs(playerHitbox, golemHitbox))
            {
                golem.TakeDamage(1);
            }

        }

        if (golem.IsDead()&& !gDeath)
            {
                Text.Create("Golem DEAD",{PlayerTest.Position.x, PlayerTest.Position.y - 30},GREEN,10);
                gDeath = true;
                
        }


        if (golem.attackDone && !PlayerTest.IsDead())
        {
            if (CheckCollisionRecs(playerHitbox, golemHitbox))
            {
                PlayerTest.TakeDamage(1);
                golem.attackDone = false;
            }

            if (PlayerTest.IsDead() && floatingTextTimer <= 0.0f)
            {
                floatingText = "You Died!";
                floatingTextPos = {PlayerTest.Position.x, PlayerTest.Position.y - 30};
                floatingTextTimer = 2.0f;
            }

        }

        
        
/////////////////////////////////// GOLEM AND PLAYER INTERACTION /////////////////////////////////////////


/////////////////////////////////// CHECK IF PLAYER AT EXIT /////////////////////////////////////////

        if(currLevel<1 &&Vector2Equals(PlayerTest.getGridPos(), Levels[currLevel].getExit())){
            currLevel++;
            PlayerTest.ResetToSpawn(Levels[currLevel].getSpawn());
            golem.SetTarget(&PlayerTest.Position);
            golem.SetMap(&Levels[currLevel]);
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

                Vector2 textPos = {PlayerTest.Position.x, PlayerTest.Position.y - 10};

                if(c->getType()=="Health"){
                    PlayerTest.setHealth(c->getValue());
                    Text.Create("+10 Health",textPos,GREEN);

                }
                
                /////////////////////////// SCORE COLLECTABLE ////////////////////////////////s
                
                else{
                    PlayerTest.setScore(c->getValue());
                    Text.Create("+10 Score",textPos,GREEN);   

                }
            }
        }

        ////////////////////////// TEXT DISPLAYING AMOUNT OF SCORE/HEALTH GAINED ///////////////////////////
        Text.Update();
        Text.Draw();

        DrawText("Move: WASD | Attack: SPACE | Exit: ESC", 10, 10, 20, RED);

        if(currLevel==1 &&Vector2Equals(PlayerTest.getGridPos(), Levels[currLevel].getExit())){
            gameOver = true;
        }

        EndMode2D();
    }
        

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(exitbackground);
    UnloadTexture(congrats);
    UnloadTexture(exitText);

    CloseWindow();
    return 0;
}
