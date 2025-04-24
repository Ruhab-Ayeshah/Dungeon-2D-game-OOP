#include <iostream>
#include <string>
#include "Player.h"
using namespace std;



Player::Player(Vector2& s){
    Sprite_sheet = LoadTexture("assets/Player_Animation_Sheet.png");
    Health = 100;
    Power = 20;

    Idle = {0,5,0,0.15,0.15};
    Attack = {0,7,0,0.07,0.07};
    Death = {0,7,0,0.12,0.12};
    MoveHorizontal = {0,7,0,0.07,0.07};
    MoveVertical = {0,3,0,0.07,0.07};

    currentAnim = &Idle;
    currentRow = 4;
    isActionPlaying = false;
    isFlipped = false;

    Position = s;
}

Player::~Player(){
    UnloadTexture(Sprite_sheet);
}

Rectangle Player::GetAnimationFrame(int Frame, int FrameRow, int FrameWidth, int FrameHeight){

    int xPos = Frame*FrameWidth;
    int YPos = FrameRow*FrameHeight;

    return {(float)xPos,(float)YPos,(float)32,(float)32};
}

void Player::Update_Animation(Animation* Anim){
    float dt = GetFrameTime();
    Anim->DurationLeft -= dt;

    if(Anim->DurationLeft<=0){
        Anim->DurationLeft = Anim->Speed;
        Anim->CurrentFrame++;

        if(Anim->CurrentFrame>Anim->LastFrame){
            Anim->CurrentFrame = Anim->FirstFrame;

            if(currentAnim==&Attack||currentAnim==&Death){
                isActionPlaying = false;
            }
        }
    }

}

void Player::SetAnimation(Animation& anim, int r){
    if(currentAnim != &anim||currentRow!=r){

        currentAnim = &anim;
        currentRow = r;

        currentAnim->CurrentFrame = anim.FirstFrame;
        currentAnim->DurationLeft = anim.Speed;
    }
}


void Player::HandleInput(Map* l){
    bool isMoving = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D);
    

        if (!this->getActionStatus()) {
            if (IsKeyPressed(KEY_SPACE)) {
                this->SetAnimation(this->getAnim("attack"), 8);
                this->setActionStatus(true);
                
            }
            else if (IsKeyPressed(KEY_K)) {
                this->SetAnimation(this->getAnim("death"), 7);
                this->setActionStatus(true);
            }
            else if (isMoving) {
                if (IsKeyDown(KEY_A)) {
                    this->SetAnimation(this->getAnim("moveH"), 3);
                    this->setFlip(true);
                    Move('L',l);
                    
                }
                else if (IsKeyDown(KEY_D)) {
                    this->SetAnimation(this->getAnim("moveH"), 3);
                    this->setFlip(false);
                    Move('R',l);
                }
                else if (IsKeyDown(KEY_W)){
                    this->SetAnimation(this->getAnim("moveV"), 2);
                    this->setFlip(false);
                    Move('U',l);
                }
                else if(IsKeyDown(KEY_S)){
                    this->SetAnimation(this->getAnim("moveV"), 2);
                    this->setFlip(false);
                    Move('D',l);
                }

            }
            else {
                this->SetAnimation(this->getAnim("idle"), 4);
            }
        }
}

void Player::Move(char n, Map* l){


        switch(n){

            case 'L':

               if(l->isWalkable((int)Position.x-1,(int)Position.y)){this->Position.x--;} 

            break;

            case 'R':

                if(l->isWalkable((int)Position.x+1,(int)Position.y)){this->Position.x++;} 

            break;

            case 'U':

                if(l->isWalkable((int)Position.x,(int)Position.y-1)){this->Position.y--;} 


            break;

            case 'D':

                if(l->isWalkable((int)Position.x,(int)Position.y+1)){this->Position.y++;} 

            break;

            default:
                return;
        }
  
}



void Player::Draw(){
    Rectangle PlayerFrame = GetAnimationFrame(getCurrentAnim()->CurrentFrame,getCurRow(),32, 32);

    if (!getFlip()) {
        DrawTexturePro(Sprite_sheet,PlayerFrame,{32*Position.x, 32*Position.y, 32, 32 },{ 0.0f, 0.0f }, 0.0f, WHITE);
    }else{
        PlayerFrame.width = -(PlayerFrame.width);
        DrawTexturePro(Sprite_sheet,PlayerFrame,{32*Position.x, Position.y, 32, 32},{0.0f,0.0f},0.0f, WHITE);
    }
}

Animation& Player::getAnim(string A){

    if(A=="idle"){
        return this->Idle;
    }else if(A=="attack"){
        return this->Attack;
    }else if(A=="death"){
        return this->Death;
    }else if(A=="moveH"){
        return this->MoveHorizontal;
    }else{
        return this->MoveVertical;
    }
}

Animation* Player::getCurrentAnim(){
    return currentAnim;
}

bool Player::getFlip(){
    return isFlipped;
}

int Player::getCurRow(){
    return currentRow;
}

bool Player::getActionStatus(){
    return isActionPlaying;
}

void Player::setActionStatus(bool x){
    isActionPlaying = x;
}

void Player::setFlip(bool x){
    isFlipped = x;
}

