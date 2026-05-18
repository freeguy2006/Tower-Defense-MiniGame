#pragma once
#include "EnemyBehavior.h"
#include "Enemy.h"

class heal_behavior : public enemy_behavior {
    private:
        float _heal_amount;
        float _heal_cooldown;
        float _heal_timer = 0;
        float _heal_range;
        bool _can_heal = false;
    public:
        heal_behavior(float amount, float cooldown, float range)
        : _heal_amount(amount), _heal_cooldown(cooldown), _heal_range(range){}
        void apply(enemy& e, float dt)override{
            _heal_timer+= dt;
            if(_heal_timer>=_heal_cooldown){
                _can_heal = true;
                _heal_timer = 0;
            }else{
                _can_heal = false;
            }
        }
        bool is_able_to_heal()const{ return _can_heal; }
        float get_heal_range()const{ return _heal_range; }
        float get_heal_amount()const{ return _heal_amount; }
        float get_heal_cooldown()const{ return _heal_cooldown; }
};