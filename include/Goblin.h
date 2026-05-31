#pragma once
#include "Enemy.h"

class goblin: public enemy{
    public:
        goblin(Vector2 position, Vector2 size, bool active, float hp, Vector2 speed, float target_x, enemy_type type, int reward)
        : enemy(position, size, active, hp, speed, target_x, type, reward) {}
};