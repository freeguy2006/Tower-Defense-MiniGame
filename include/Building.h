// 建築基底 (is-a game_object, has-a health)
// 繼承自 game_object: get_position(), get_size(), set_position(), is_active(), get_rect()
// 來自 health (has-a): get_hp(), is_alive(), take_damage(float)
#pragma once
#include "Health.h"
#include "GameObject.h"
class building : public game_object{
    private:
        health _health;
    public:
        building(Vector2 position, Vector2 size, bool active, float hp):game_object(position, size, active),_health(hp){}
        ~building() = default;
        float get_hp() const { return _health.get_hp(); }
        float get_max_hp() const { return _health.get_max_hp(); }
        bool is_alive() const { return _health.is_alive(); }
        void heal(float amount){ _health.heal(amount); }
        void take_damage(float damage){ _health.take_damage(damage); }
        void increase_max_hp(float amount){ _health.increase_max_hp(amount); }
};