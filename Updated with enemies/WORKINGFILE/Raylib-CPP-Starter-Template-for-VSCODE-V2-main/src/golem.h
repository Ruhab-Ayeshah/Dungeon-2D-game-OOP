#ifndef GOLEM_H
#define GOLEM_H

#include "Enemy.h"
#include <string>
#include "Map.h"
class Golem : public Enemy
{
    float moveSpeed;

    Vector2 *PlayerPosition;
    float WanderTimer;
    int HitsTaken;
    float patrolStartX;

    float patrolEndX;
    int patrolDirection; // 1 for right, -1 for left

public:
    Golem(Vector2 position);
    ~Golem();
    Map *map; // Pointer to the map
    void SetMap(Map *m);
    bool attackDone;
    std::string State;
    int patrolStartTileX;
    int patrolEndTileX;
    void InitializePatrol();
    bool CanMoveTo(float nextX, float nextY);
    void Update(float dt) override;
    void Draw() override;
    void TakeDamage(int amount) override;
    bool IsDead() override;

    void SetTarget(Vector2 *playerPos);

private:
    void AI_Behavior(float dt);
};

#endif