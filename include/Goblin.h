#pragma once
#include "Character.h"

class goblin:public character{
    private:
        int _reward;
    public:
        goblin(Vector2 position, Vector2 size, bool active,float hp, Vector2 speed, int reward)
        : character(position, size, active, hp, speed), _reward(reward){}
        int get_reward() const { return _reward; }
        void update(float dt) override {
            Vector2 pos = get_position();
            pos.x += get_speed().x * dt;
            set_position(pos);
        }

};