#pragma once
#include <vector>
#include <cmath>
#include "Player.h"
#include "Enemy.h"
#include "Castle.h"
#include "Projectile.h"
#include "GameFactory.h"

enum game_statement{START,PLAYING,PAUSE,LOSE};

class game{
    private:
        player _player; 
        castle _castle;
        float _enemy_spawn_timer = 0;
        float _enemy_spawn_cooldown = 1.0;  
        int _kill_count = 0; // the number of enemy killed
        game_statement _game_statement = START;
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