// 玩家 (is-a character)
// 繼承可用: get_position(), set_position(), get_size(), is_active(), get_rect()
//          get_hp(), is_alive(), take_damage(float), get_speed(), set_speed()
// 自己的: get_gravity(), get_jump_force(), get_ground_y(), get_move_speed(), is_on_ground()
#pragma once
#include "Character.h"

class player : public character{
    private:
        float _attack_timer;
        float _attack_cooldown;
        float _attack_base_cooldown;
        float _gravity;
        float _jump_force;
        float _ground_y;
        bool _facing_right = true;
        float _move_speed;
        float _move_base_speed;
    public:
        player(Vector2 position, Vector2 size, bool active, float hp, Vector2 speed,float atkcooldown,float gravity,float jump_force, float ground_y, float move_speed):character(position, size, active, hp, speed),_attack_timer(0), _attack_cooldown(atkcooldown), _attack_base_cooldown(atkcooldown),_gravity(gravity),_jump_force(jump_force),_ground_y(ground_y), _move_speed(move_speed), _move_base_speed(move_speed){}
        ~player() = default;
        float get_gravity()const { return _gravity; }
        float get_jump_force()const { return _jump_force; }
        float get_ground_y()const { return _ground_y; }
        float get_move_speed()const { return _move_speed; }
        float get_attack_cooldown()const { return _attack_cooldown; }
        float get_base_cooldown() const { return _attack_base_cooldown; }
        float get_move_base_speed() const { return _move_base_speed; }


        bool is_facing_right()const { return _facing_right; }
        bool is_attackable()const { return _attack_timer>=_attack_cooldown; }
        bool is_on_ground()const { return get_position().y >= _ground_y; }


        void decrease_cooldown(float amount){
            _attack_cooldown -= amount;
            if(_attack_cooldown < 0.05f) _attack_cooldown = 0.05f;  // 最低 0.05 秒
        }

        void increase_move_speed(float amount){ _move_speed += amount; }
        void reset_attack_timer(){ _attack_timer = 0; }
        void set_attack_cooldown(float cd){ _attack_cooldown = cd; }
        void set_move_speed(float spd) { _move_speed = spd; }
        void update(float dt) override {
            _attack_timer += dt;
            if(IsKeyDown(KEY_A)){
                set_speed({-_move_speed, get_speed().y});
                _facing_right = false;
            }else if(IsKeyDown(KEY_D)){
                set_speed({_move_speed, get_speed().y});
                _facing_right = true;
            }else{
                set_speed({0, get_speed().y});
            }
            if(IsKeyPressed(KEY_W) && is_on_ground()){
                set_speed({get_speed().x, -_jump_force});
            }
            Vector2 temp = get_position();
            set_speed({get_speed().x, get_speed().y + _gravity * dt});
            temp.x += get_speed().x * dt;
            temp.y += get_speed().y * dt;
            if(temp.y >= _ground_y){
                temp.y = _ground_y;
                set_speed({get_speed().x, 0});
            }
            // 邊界
            if(temp.x<0){
                temp.x = 0;
            }
            if(temp.x>2400-get_size().x){
                temp.x = 2400-get_size().x;
            }
            set_position(temp);
        }

};