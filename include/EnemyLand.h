#pragma once
#include "Enemy.h"
class land_enemy: public enemy{
    public:
        land_enemy(Vector2 position, Vector2 size, bool active, float hp, Vector2 speed, float target_x, enemy_type type, int reward)
        : enemy(position, size, active, hp, speed, target_x, type, reward) {}
};