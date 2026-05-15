#pragma once
#include <vector>
#include <cmath>
#include "Player.h"
#include "Enemy.h"
#include "Castle.h"
#include "Projectile.h"
#include "GameFactory.h"


class game{
    private:
        player _player;
        castle _castle;
        std::vector<enemy> _enemies;
        std::vector<projectile> _projectiles;
    public:
        game(player p, castle c):_player(p),_castle(c){} 
        ~game() = default;

        int get_enemies_size(){ return _enemies.size(); }

        void update(float dt);
        void draw(); 
        void run();
        void add_enemy(enemy e){
            _enemies.push_back(e);
        }
};