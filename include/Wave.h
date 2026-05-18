#pragma once
#include "Enemy.h"
#include <vector>

class wave{
    private:
        std::vector<enemy_type> _coming_enemies;
        float _spawn_cooldown;
    public:
        wave(std::vector<enemy_type> coming_enemies, float cooldown):_coming_enemies(coming_enemies),_spawn_cooldown(cooldown){}
        ~wave() = default;
        std::vector<enemy_type> get_coming_enemies() const { return _coming_enemies; }
        float get_spawn_cooldown() const { return _spawn_cooldown; }
        int get_total_enemies() const { return _coming_enemies.size(); }

};