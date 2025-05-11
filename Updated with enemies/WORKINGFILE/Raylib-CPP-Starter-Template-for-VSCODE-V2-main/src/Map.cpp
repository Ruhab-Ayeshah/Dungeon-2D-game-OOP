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

Map::Map(){}

Map::Map(const string& filename,int w, int f, int e,char const* tileFile){
    tileset = LoadTexture(tileFile);
    exitTexture = LoadTexture("assets/Map_Assets/exit.png");
    tileSize = 32;
    tilesetHeight = tileset.height;
    tilesetWidth = tileset.width;
    tilesPerRow = tilesetWidth/tileSize;

    wallID = w;
    floorID = f;
    exitID = e;

    loadFile(filename);

    
    
}

Map::~Map(){
    UnloadTexture(tileset);
    UnloadTexture(exitTexture);
}

void Map::loadFile(const string& filename){

    ifstream MapFile(filename);
    if(!MapFile.is_open()){
        cout<<"Failed to load file"<<endl;
        return;
    }

    int y = 0;
    string line;

    while(getline(MapFile,line)&&y<20){
        for(int x=0;x<line.length()&&x<30;x++){
            char c = line[x];

            switch(c){

                case '#':
                    if(y==0 && x==0){                                                  //Top-left corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,wallID}; 
                    }else if(y==0 && x==29){                                           //Top-right corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }else if(y==19 && x==0){                                           //Bottom-left corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }else if(y==19 && x==29){                                          //Bottom-right corner tile
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }else if(x==0){                                                    //Left side wall
                        
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};

                    }else if(x==29){                                                   //Right side wall
                        
                        
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};

                    }else if(y==0){                                                    //Top of map
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }else if(y==19){                                                   //Bottom of map
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }else{                                                             //All walls inside the map
                        
                        CompleteMap[y][x] = {false,false,false,false,false,wallID};
                    }
                    
                    break;
                
                case '.':
                    CompleteMap[y][x] = {true,false,false,false,false,floorID};
                    break;

                case 'P':
                    CompleteMap[y][x] = {true,false,false,false,true,floorID};
                    Spawn = {(float)x,(float)y};
                    break;

                case 'E':
                    CompleteMap[y][x] = {true,true,false,false,false,exitID};
                    Exit = {(float)x,(float)y};
                    break;

                case '$':
                    CompleteMap[y][x] = {true,false,false,true,false,floorID};
                break;

                case 'X':
                    CompleteMap[y][x] = {true,false,true,false,false,floorID};
                break;

                default:
                    CompleteMap[y][x] = {true,false,false,false,false,floorID};
                
            }
        }
        y++;
    }

    MapFile.close();

}

void Map::setTile(int x, int y, bool isW, bool isE, bool isX, bool isC, bool isS, int ID){
    if(y<20&&y>=0&&x>0&&x<30){
        CompleteMap[y][x]={isW,isE,isX,isC,isS,ID};
    }
}

bool Map::isWalkable(int x, int y){
    if(y<20&&y>=0&&x>0&&x<30){
        return CompleteMap[y][x].walkable;
    }else{
        return false;
    }
}

void Map::Draw(){

    for(int y=0;y<20;y++){
        for(int x=0;x<30;x++){
            Rectangle source = getTileRec(CompleteMap[y][x].tileID);
            Vector2 newPos = {(float)x*32,(float)y*32};
            if(CompleteMap[y][x].tileID==34){
                DrawTextureRec(exitTexture, {0,0,(float)tileSize,(float)tileSize},newPos,WHITE);
            }else{
            DrawTextureRec(tileset,source,newPos,WHITE);
            }
        }
    }
}



Vector2& Map::getSpawn(){
    return Spawn;
}

Vector2& Map::getExit(){
    return Exit;
}

Tile& Map::getTile(int x, int y) {
    return CompleteMap[y][x];
}