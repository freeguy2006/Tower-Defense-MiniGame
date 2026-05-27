#pragma once
#include "Enemy.h"

class sky_enemy: public enemy{
    private:
        float _fly_timer = 0;
        float _fly_amplitude;
        float _fly_speed;
        float _base_y;
    public:
        sky_enemy(Vector2 position, Vector2 size, bool active, float hp, Vector2 speed, float target_x, enemy_type type, float amplitude, float fly_speed, int reward)
        : enemy(position, size, active, hp, speed, target_x, type, reward), _fly_amplitude(amplitude), _fly_speed(fly_speed), _base_y(position.y) {}
        void update(float dt) override {
            if (update_status(dt)) return;
            if (get_position().x > get_target_x()) {
                Vector2 temp = get_position();
                temp.x += get_speed().x * get_slow_multiplier() * dt;
                _fly_timer += dt;
                temp.y = _base_y + sin(_fly_timer * _fly_speed) * _fly_amplitude;
                set_position(temp);
            }
            for (auto b : get_behaviors()) {  // apply behaviors
                b->apply(*this, dt);
            }
        }
};