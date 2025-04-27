#include "golem.h"
#include "raymath.h"
#include "raylib.h"
#include "Map.h"
Golem::Golem(Vector2 position)
{
    Position = position;
    Health = 150;

    SpriteSheet = LoadTexture("assets/Fire Golem.png");

    Idle = {0, 3, 0, 0.25f, 0.25f};
    Walk = {0, 7, 0, 0.1f, 0.1f};
    Attack = {0, 7, 0, 0.15f, 0.15f};
    Die = {0, 6, 0, 0.2f, 0.2f};
    // 4 tiles right
    patrolDirection = 1; // start moving right

    CurrentAnim = &Idle;
    CurrentRow = 0;
    isActionPlaying = false;
    isFlipped = false;

    moveSpeed = 50.0f;
    State = "idle";

    PlayerPosition = nullptr;
    WanderTimer = 0.0f;
    HitsTaken = 0;

    patrolStartTileX = (int)(Position.x / 32) - 4;
    patrolEndTileX = (int)(Position.x / 32) + 4;
    patrolDirection = 1;

    patrolDirection = 1;

    map = nullptr;

    attackDone = false;
}

Golem::~Golem()
{
    UnloadTexture(SpriteSheet);
}

void Golem::SetTarget(Vector2 *playerPos)
{
    PlayerPosition = playerPos;
}

void Golem::SetMap(Map *m)
{
    map = m;
}

bool Golem::CanMoveTo(float nextX, float nextY)
{
    float left = nextX;
    float right = nextX + 64 - 1;
    float top = nextY;
    float bottom = nextY + 64 - 1;

    int leftTile = Clamp((int)(left / 32), 0, 29);
    int rightTile = Clamp((int)(right / 32), 0, 29);
    int topTile = Clamp((int)(top / 32), 0, 19);
    int bottomTile = Clamp((int)(bottom / 32), 0, 19);

    return (map &&
            map->isWalkable(leftTile, topTile) &&
            map->isWalkable(rightTile, topTile) &&
            map->isWalkable(leftTile, bottomTile) &&
            map->isWalkable(rightTile, bottomTile));
}

void Golem::AI_Behavior(float dt)
{
    if (State == "die")
        return;

    float distance = Vector2Distance(Position, *PlayerPosition);

    if (State != "attack")
    {
        if (distance <= 40)
        {
            isFlipped = (PlayerPosition->x < Position.x);

            SetAnimation(Attack, 4);
            State = "attack";
            isActionPlaying = true;
            attackDone = false;
            return;
        }
        else if (distance <= 150)
        {
            // Chasing Player
            Vector2 direction = Vector2Normalize({PlayerPosition->x - Position.x, 0.0f});
            float nextX = Position.x + direction.x * moveSpeed * dt;

            if (CanMoveTo(nextX, Position.y))
            {
                Position.x += direction.x * moveSpeed * dt;
                isFlipped = direction.x < 0;
            }
            SetAnimation(Walk, 1);
            State = "chase";
        }
        else
        {
            // Patrolling between 4 tiles
            float nextX = Position.x + patrolDirection * moveSpeed * dt;

            int nextTileX = (int)(nextX / 32);

            if (nextTileX < patrolStartTileX || nextTileX > patrolEndTileX)
            {
                patrolDirection *= -1;
            }

            if (CanMoveTo(nextX, Position.y))
            {
                Position.x += patrolDirection * moveSpeed * dt;
            }
            else
            {
                patrolDirection *= -1; // wall
            }

            isFlipped = patrolDirection < 0;
            SetAnimation(Walk, 1);
            State = "patrol";
        }
    }

    if (State == "attack" && !isActionPlaying)
    {
        attackDone = true;
        State = "chase";
    }

    Position.x = Clamp(Position.x, 0.0f, (30 * 32) - 64);
    Position.y = Clamp(Position.y, 0.0f, (20 * 32) - 64);
}

void Golem::Update(float dt)
{
    if (PlayerPosition)
    {
        AI_Behavior(dt);
    }
    UpdateAnimation(CurrentAnim);
}

void Golem::Draw()
{
    Rectangle frame = {
        (float)(CurrentAnim->CurrentFrame * 64),
        (float)(CurrentRow * 64),
        64,
        64};

    Rectangle dest = {Position.x, Position.y, 64, 64};
    Vector2 origin = {0, 0};

    if (!isFlipped)
    {
        DrawTexturePro(SpriteSheet, frame, dest, origin, 0, WHITE);
    }
    else
    {
        frame.width = -frame.width;
        DrawTexturePro(SpriteSheet, frame, dest, origin, 0, WHITE);
    }
}

void Golem::TakeDamage(int amount)
{
    if (State == "die")
        return;

    HitsTaken++;
    if (HitsTaken >= 3)
    {
        SetAnimation(Die, 7);
        State = "die";
        isActionPlaying = true;
    }
    else
    {
        SetAnimation(Attack, 4);
        State = "attack";
        isActionPlaying = true;
        attackDone = false;
    }
}

bool Golem::IsDead()
{
    return (State == "die") && !isActionPlaying;
}
