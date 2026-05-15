// 玩家 (is-a character)
// 繼承可用: get_position(), set_position(), get_size(), is_active(), get_rect()
//          get_hp(), is_alive(), take_damage(int), get_speed(), set_speed()
// 自己的: get_gravity(), get_jump_force(), get_ground_y(), get_move_speed(), is_on_ground()
#pragma once
#include "Character.h"

class player : public character{
    private:
        float _attack_timer;
        float _attack_cooldown;
        float _gravity;
        float _jump_force;
        float _ground_y;
        float _move_speed;
    public:
        player(Vector2 position, Vector2 size, bool active, int hp, Vector2 speed,float atkcooldown,float gravity,float jump_force, float ground_y, float move_speed):character(position, size, active, hp, speed),_attack_timer(0), _attack_cooldown(atkcooldown),_gravity(gravity),_jump_force(jump_force),_ground_y(ground_y), _move_speed(move_speed){}
        ~player() = default;
        float get_gravity()const { return _gravity; }
        float get_jump_force()const { return _jump_force; }
        float get_ground_y()const { return _ground_y; }
        float get_move_speed()const { return _move_speed; }
        
        bool is_attackable()const { return _attack_timer>=_attack_cooldown; }
        bool is_on_ground()const { return get_position().y >= _ground_y; }
        
        void reset_attack_timer(){ _attack_timer = 0; }
        void update(float dt) override;        
    
};