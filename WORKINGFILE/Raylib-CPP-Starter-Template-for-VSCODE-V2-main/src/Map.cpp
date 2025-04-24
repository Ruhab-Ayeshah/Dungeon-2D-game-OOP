#include "Map.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

Rectangle Map::getTileRec(int TileID){
    int xCoord = (TileID%tilesPerRow)*tileSize;
    int yCoord = (TileID/tilesPerRow)*tileSize;

    Rectangle temp = {(float)xCoord, (float)yCoord,(float)tileSize,(float)tileSize};
    return temp;
}


Map::Map(const string& filename){
    tileset = LoadTexture("assets/Map_Assets/Dungeon_Bricks_Shadow.png");
    tileSize = 32;
    tilesetHeight = tileset.height;
    tilesetWidth = tileset.width;
    tilesPerRow = tilesetWidth/tileSize;

    for(int i=0;i<30;i++){
        for(int j=0;j<40;j++){
            ItemOverlay[i][j]=false;
        }
    }


    loadFile(filename);

    minerals[0] = LoadTexture("assets/Map_Assets/Minerals/Icon1.png");
    minerals[1] = LoadTexture("assets/Map_Assets/Minerals/Icon2.png");
    minerals[2] = LoadTexture("assets/Map_Assets/Minerals/Icon3.png");
    minerals[3] = LoadTexture("assets/Map_Assets/Minerals/Icon4.png");
    minerals[4] = LoadTexture("assets/Map_Assets/Minerals/Icon5.png");
    minerals[5] = LoadTexture("assets/Map_Assets/Minerals/Icon6.png");
    minerals[6] = LoadTexture("assets/Map_Assets/Minerals/Icon7.png");
    minerals[7] = LoadTexture("assets/Map_Assets/Minerals/Icon8.png");
    minerals[8] = LoadTexture("assets/Map_Assets/Minerals/Icon9.png");
    minerals[9] = LoadTexture("assets/Map_Assets/Minerals/Icon10.png");

    shadow = LoadTexture("assets/Map_Assets/shadow.png");

    
}

Map::~Map(){
    UnloadTexture(tileset);
    for(int i=0;i<10;i++){
        UnloadTexture(minerals[i]);
    }
    UnloadTexture(shadow);
}

void Map::loadFile(const string& filename){

    ifstream MapFile(filename);
    if(!MapFile.is_open()){
        cout<<"Failed to load file"<<endl;
        return;
    }

    int y = 0;
    string line;

    while(getline(MapFile,line)&&y<30){
        for(int x=0;x<line.length()&&x<40;x++){
            char c = line[x];

            switch(c){

                case '#':
                    if(y==0 && x==0){                                                  //Top-left corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,4}; 
                    }else if(y==0 && x==39){                                           //Top-right corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,4};
                    }else if(y==29 && x==0){                                           //Bottom-left corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,4};
                    }else if(y==29 && x==39){                                          //Bottom-right corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,4};
                    }else if(x==0){                                                    //Left side wall
                        
                        int random[4] = {8,14,38,36};
                        
                        CompleteMap[y][x] = {false,false,false,false,false,random[GetRandomValue(0,3)]};

                    }else if(x==39){                                                   //Right side wall
                        
                        int random1[4] = {10,15,39,37};
                        
                        CompleteMap[y][x] = {false,false,false,false,false,random1[GetRandomValue(0,3)]};

                    }else if(y==0){                                                    //Top of map
                        CompleteMap[y][x] = {false,false,false,false,false,4};
                    }else if(y==29){                                                   //Bottom of map
                        CompleteMap[y][x] = {false,false,false,false,false,4};
                    }else{                                                             //All walls inside the map
                        
                        CompleteMap[y][x] = {false,false,false,false,false,35};
                    }
                    
                    break;
                
                case '.':
                    CompleteMap[y][x] = {true,false,false,false,false,9};
                    break;

                case 'P':
                    CompleteMap[y][x] = {true,false,false,false,true,9};
                    Spawn = {(float)x,(float)y};
                    break;

                case 'E':
                    CompleteMap[y][x] = {true,true,false,false,false,9};
                    break;

                case '$':
                    CompleteMap[y][x] = {true,false,false,true,false,9};
                break;

                case 'X':
                    CompleteMap[y][x] = {true,false,true,false,false,9};
                break;

                case 'M':
                    CompleteMap[y][x] = {true,false,false,false,false,9};
                    ItemOverlay[y][x] = true;
                    CompleteMap[y][x].ItemID = GetRandomValue(0,9);
                    
                    
                break;

                default:
                    CompleteMap[y][x] = {true,false,false,false,false,9};
                
            }
        }
        y++;
    }

    MapFile.close();

}

void Map::setTile(int x, int y, bool isW, bool isE, bool isX, bool isC, bool isS, int ID){
    if(y<30&&y>=0&&x>0&&x<40){
        CompleteMap[y][x]={isW,isE,isX,isC,isS,ID};
    }
}

bool Map::isWalkable(int x, int y){
    if(y<30&&y>=0&&x>0&&x<40){
        return CompleteMap[y][x].walkable;
    }else{
        return false;
    }
}

void Map::Draw(){

    for(int y=0;y<30;y++){
        for(int x=0;x<40;x++){
            Rectangle source = getTileRec(CompleteMap[y][x].tileID);
            Vector2 newPos = {(float)x*32,(float)y*32};
            
            DrawTextureRec(tileset,source,newPos,WHITE);
            
        }
    }
}

void Map::DrawItems(){

    for(int y=0;y<30;y++){
        for(int x=0;x<40;x++){
            Vector2 newPos = {(float)x*32,(float)y*32};

            if(ItemOverlay[y][x]==true){
                DrawTexture(shadow,newPos.x,newPos.y,WHITE);
                DrawTexture(minerals[CompleteMap[y][x].ItemID],newPos.x,newPos.y,WHITE);
            }
            
        }
    }
}

Vector2& Map::getSpawn(){
    return Spawn;
}



Tile& Map::getTile(int x, int y) {
    return CompleteMap[y][x];
}