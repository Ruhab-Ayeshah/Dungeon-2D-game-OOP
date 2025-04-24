#include "Map.h"
#include "Player.h"
#include <raylib.h>
using namespace std;

int main() {
    InitWindow(1280, 960, "Map Test");
    SetTargetFPS(60);
    string f="assets/Map_Assets/Level1.txt";
    Map Level1(f);
    Player PlayerTest(Level1.getSpawn());
    
    

    while (!WindowShouldClose()) {

        PlayerTest.HandleInput(&Level1);
        PlayerTest.Update_Animation(PlayerTest.getCurrentAnim());
        BeginDrawing();
        ClearBackground(BLACK);
        Level1.Draw();

        
        PlayerTest.Draw();

       // Level1.DrawItems();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
