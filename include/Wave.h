#pragma once
#include "Enemy.h"
#include <vector>

struct wave_data{
    enemy_type type;
    float cooldown;
};

class wave{
    private:
        std::vector<wave_data> _coming_enemies;
    public:
        wave(std::vector<wave_data> coming_enemies):_coming_enemies(coming_enemies){}
        ~wave() = default;
        std::vector<wave_data> get_coming_enemies() const { return _coming_enemies; }
        int get_total_enemies() const { return _coming_enemies.size(); }
        float get_spawn_cooldown(int x) const { return _coming_enemies[x].cooldown; } // 接收 x: 那一波的第幾段 
        enemy_type get_type(int x) const { return _coming_enemies[x].type;} 
};