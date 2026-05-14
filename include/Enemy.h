// 敵人 (is-a character)，從右往左走到 target_x 停下
// 繼承可用: get_position(), set_position(), get_size(), is_active(), get_rect()
//          get_hp(), is_alive(), take_damage(int), get_speed(), set_speed()
#pragma once
#include "Character.h"
class enemy : public character{
    private:
        int _target_x;
    public:
        enemy(Vector2 position, Vector2 size, bool active, int hp, Vector2 speed, int target_x) : character(position, size, active, hp, speed), _target_x(target_x) {}
        ~enemy() = default;
        void update(float dt) override {
            if (get_position().x > _target_x) {
                Vector2 temp = get_position();
                temp.x += get_speed().x * dt;
                set_position(temp);
            }
        }
        
};