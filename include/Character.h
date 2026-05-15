// 角色基底 (is-a game_object, has-a health)
// 繼承自 game_object: get_position(), get_size(), set_position(), is_active(), get_rect()
// 來自 health (has-a): get_hp(), is_alive(), take_damage(int)
// 自己的: get_speed(), set_speed(Vector2)
#pragma once
#include "Health.h"
#include "GameObject.h"
class character: public game_object{
    private:
        Vector2 _speed;
        health _health;
    public:
        character(Vector2 position, Vector2 size, bool active, int hp, Vector2 speed):game_object(position, size, active), _health(hp), _speed(speed){}
        ~character() = default;
        int get_hp() const { return _health.get_hp(); }
        int get_max_hp() const { return _health.get_max_hp(); }
        bool is_alive() const { return _health.is_alive(); }
        void take_damage(int damage){ _health.take_damage(damage); }
        Vector2 get_speed()const{ return _speed; }
        void set_speed(Vector2 speed){ _speed = speed; }
};