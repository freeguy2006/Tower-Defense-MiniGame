#pragma once
#include "GameObject.h"
class falling_object : public game_object {
    protected:
        Vector2 _speed;
        float _gravity;
        float _ground_y;
    public:
        falling_object(Vector2 position, Vector2 size, bool active, float ground_y, float gravity, Vector2 initial_speed = {0, 0})
        : game_object(position, size, active), _ground_y(ground_y), _gravity(gravity), _speed(initial_speed) {}
        
        virtual ~falling_object() = default;
        void update(float dt) override { update_physics(dt); }
        // update phyics
        void update_physics(float dt) { // 重力
            Vector2 pos = get_position();
            _speed.y += _gravity * dt;
            pos.x += _speed.x * dt;
            pos.y += _speed.y * dt;
            if (pos.y >= _ground_y) {
                pos.y = _ground_y;
                _speed.y = 0;
                _speed.x = 0;
            }
            set_position(pos);
        }
};
