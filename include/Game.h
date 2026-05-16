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
        Texture2D _enemy_texture;
        Texture2D _projectile_texture;
        Texture2D _castle_texture;
        
        std::vector<enemy> _enemies;
        std::vector<projectile> _projectiles;
    public:
        game(player p, castle c):_player(p),_castle(c){
            _enemy_texture = LoadTexture("resources/monster/monster_3.png");
            _projectile_texture = LoadTexture("resources/ammo/ammo_3.png");
            _castle_texture = LoadTexture("resources/castle/castle.png");
            //_player_texture = LoadTexture("resources/player/player.png");
        } 
        ~game(){    
            UnloadTexture(_enemy_texture);
            UnloadTexture(_projectile_texture);
            UnloadTexture(_castle_texture);
            //UnloadTexture(_player_texture);
        };

        int get_enemies_size(){ return _enemies.size(); }

        void update(float dt);
        void draw(); 
        void run();
        void add_enemy(enemy e){
            _enemies.push_back(e);
        }
};