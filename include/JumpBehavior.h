#pragma once
#include "EnemyBehavior.h"
#include "Enemy.h"
#include <cmath>
class jump_behavior : public enemy_behavior{
    private:
        float _base_y;
        float _vertical_speed = 0;
        float _small_jump_force = 0;
        float _big_jump_force = 0;
        float _gravity = 1600;
        float _jump_timer = 0;
        float _jump_cooldown;
        int _jump_count = 0;
    public:
        jump_behavior(float base_y,float jump_force, float cooldown):_base_y(base_y),_small_jump_force(jump_force),_jump_cooldown(cooldown){}
        jump_behavior(float base_y,float small_jump_force, float big_jump_force ,float cooldown):_base_y(base_y),_small_jump_force(small_jump_force),_big_jump_force(big_jump_force),_jump_cooldown(cooldown){}
        
        void apply(enemy &e, float dt) override{
            if(e.is_frozen()) return;
            float slow = e.get_slow_multiplier();
            Vector2 enemy_pos = e.get_position();
            _jump_timer += dt * slow;
            if(_jump_timer>=_jump_cooldown && enemy_pos.y >= _base_y){
                _jump_timer = 0;
                if(_jump_count%2==0 || _big_jump_force == 0){
                    _vertical_speed = -_small_jump_force;
                }else{
                    _vertical_speed = -_big_jump_force;
                }
                _jump_count++;
            }
            _vertical_speed += _gravity * dt * slow;
            enemy_pos.y += _vertical_speed * dt * slow;
            if(enemy_pos.y >= _base_y){
                enemy_pos.y = _base_y;
                _vertical_speed = 0;
            }
            e.set_position(enemy_pos);
        }
};