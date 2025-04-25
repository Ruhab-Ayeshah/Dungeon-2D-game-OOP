#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include "raylib.h"
using namespace std;

struct Tile{
    bool walkable;
    bool exit;
    bool enemy;
    bool collectable;
    bool spawn;
    int tileID;
    int ItemID = -1;
};

class Map{

    int tileSize;         
    int tilesetWidth;     
    int tilesetHeight;    
    int tilesPerRow;
    Vector2 Spawn;   

    Tile CompleteMap[20][30];
    Texture2D tileset;
    // Texture2D minerals[10];
    // Texture2D shadow = LoadTexture("assets/Map_Assets/shadow.png");

    // bool ItemOverlay[30][40];
    
    Rectangle getTileRec(int TileID);
    public:

        Map(const string& filename);
        ~Map();
        void Draw();
        void DrawItems();
        void loadFile(const string& filename);
        void setTile(int x, int y, bool isW, bool isE, bool isX, bool isC, bool isS, int ID);
        bool isWalkable(int x, int y);

        Vector2& getSpawn();

        Tile& getTile(int x, int y);

};

#endif