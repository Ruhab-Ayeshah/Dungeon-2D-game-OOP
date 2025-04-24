#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include "raylib.h"
#include "Map.h"

using namespace std;

struct Animation{
    int FirstFrame;
    int LastFrame;
    int CurrentFrame;

    float Speed;
    float DurationLeft;
};

class Player{

    

    int Health;
    int Power;

    Animation Idle;
    Animation Attack;
    Animation Death;
    Animation MoveHorizontal;
    Animation MoveVertical;
    Animation* currentAnim;
    bool isActionPlaying;
    bool isFlipped;
    int currentRow;

    
    public:
    Vector2 Position;
        Texture2D Sprite_sheet;

        Player(Vector2& s);
        ~Player();

        void Update_Animation(Animation* Anim);
        Rectangle GetAnimationFrame(int Frame, int FrameRow, int FrameWidth, int FrameHeight);
        void SetAnimation(Animation& anim, int r);
        
        Animation& getAnim(string A);
        Animation* getCurrentAnim();
        bool getFlip();
        int getCurRow();
        bool getActionStatus();
        void setActionStatus(bool x);
        void setFlip(bool x);

        void Draw();
        void HandleInput(Map* l);

        void Move(char n, Map* l);


};

#endif