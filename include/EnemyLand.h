#pragma once
#include "Enemy.h"
class land_enemy: public enemy{
    public:
        land_enemy(Vector2 position, Vector2 size, bool active, float hp, Vector2 speed, float target_x, enemy_type type, int reward)
        : enemy(position, size, active, hp, speed, target_x, type, reward) {}
        void update(float dt)override{ // from enemy
            if(update_status(dt)) return; // 確認 freeze
            if (get_position().x > get_target_x()) {
                Vector2 temp = get_position();
                temp.x += get_speed().x * get_slow_multiplier() * dt;
                set_position(temp);
            }
            for (auto b : get_behaviors()) {  // apply behaviors
                b->apply(*this, dt);
            }
        }
};