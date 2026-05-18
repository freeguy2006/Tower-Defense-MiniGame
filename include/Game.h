#pragma once
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include "Player.h"
#include "Enemy.h"
#include "Castle.h"
#include "FlyingEnemy.h"
#include "Wave.h"
#include "Projectile.h"
#include "GameFactory.h"
#include "BuffBehavior.h"
#include "JumpBehavior.h"

enum game_statement{START,PLAYING,PAUSE,WIN,LOSE};


class game{
    private:
        // objects
        player _player;
        castle _castle;
        std::vector<enemy*> _enemies;
        std::vector<projectile> _projectiles;
        std::vector<wave> _waves;
        int _current_wave = 0;
        int _enemies_spawned = 0;
        bool _is_wave_active = false;
        float _wave_rest_timer = 0; 
        float _wave_rest_duration = 5;
        // details
        float _enemy_spawn_timer = 0;
        float _enemy_spawn_cooldown = 1.0;
        int _kill_count = 0; // the number of enemy killed
        game_statement _game_statement = START;
        
        // textures
        Texture2D _enemy_green_texture;
        Texture2D _enemy_black_texture;
        Texture2D _enemy_red_texture;
        Texture2D _enemy_purple_texture;
        Texture2D _enemy_blue_texture;
        Texture2D _enemy_angel_texture;
        Texture2D _enemy_bird_texture;
        Texture2D _enemy_dragon_texture;
        Texture2D _projectile_texture;
        Texture2D _castle_texture;
        Texture2D _background_texture;
        Texture2D _player_texture;
        
    public:
        game();
        ~game() = default;
        void init();
        void close();
        void reset();
        int get_enemies_size() const { return _enemies.size(); }

        void update(float dt);
        void draw();
        void run();
        void add_enemy(enemy* e){ _enemies.push_back(e); }
        void load_waves(const char* path);
};