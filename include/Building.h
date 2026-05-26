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