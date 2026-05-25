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
#include "HealBehavior.h"
#include "Coin.h"
#include "Goblin.h"
#include "DamageText.h"

enum game_statement{START,TUTORIAL,PLAYING,PAUSE,WAVE_SHOP,WIN,LOSE};


struct potion_effect{
    float value;
    float default_value;
    int waves = 0;
};

class game{
    private:
        // objects
        player _player;
        castle _castle;
        std::vector<enemy*> _enemies;
        std::vector<projectile> _projectiles;
        std::vector<wave> _waves;
        std::vector<coin> _coins;
        // weapon
        weapon_type _current_weapon = MUD;
        // waveshop
        
        // wave
        bool _is_wave_active = false;
        float _wave_rest_timer = 0; 
        float _wave_rest_duration = 4.0f;
        int _current_wave = 0;        
        // player
        float _player_damage = PLAYER_VALUES.damage;
        float _player_crit_chance = 0.0f;
        float _player_crit_multiplier = 0.0f;
        int _multi_shot = 1;
        // enemy
        int _enemies_spawned = 0;
        float _enemy_spawn_timer = 0;
        float _enemy_spawn_cooldown = 1.0f;
        int _kill_count = 0; 
        // statement
        game_statement _game_statement = START;
        void handle_start();
        void handle_tutorial();
        void handle_playing();
        void handle_pause();
        void handle_wave_shop();
        void handle_end();
        // golds
        int _golds = PLAYER_VALUES.start_golds;
        int _max_golds = PLAYER_VALUES.max_golds;
        // player_levels
        int _player_level[9] = {0};
        // debug
        bool _debug_hitbox = false;
        // details
        float _press_delay = 0;
        // announce
        float _announce_wave_timer = 0;
        float _announce_wave_duration = 3.0f;
        bool _is_announcing_wave = false;

        // tutorialdd
        int _tutorial_page = 0;
        // shop
        goblin* _shop_goblin = nullptr;
        weapon_type _shop_weapon1 = MUD;
        weapon_type _shop_weapon2 = MUD;
        potion_type _shop_potion = HEAL_PLAYER_POTION;

        // potion effects
        potion_effect _buffs[BUFF_COUNT] = {
            {1.0f, 1.0f, 0},  // BUFF_ATTACK
            {1.0f, 1.0f, 0},  // BUFF_ATTACK_SPEED
            {1.0f, 1.0f, 0},  // BUFF_MOVE_SPEED
            {0, 0, 0},        // BUFF_REGEN
            {1.0f, 1.0f, 0},  // BUFF_SHIELD
        };
        Texture2D _buff_textures[BUFF_COUNT];

        // textures
        
        Texture2D _castle_texture;
        Texture2D _background_texture;
        Texture2D _player_texture;
        Texture2D _coin_texture;
        Texture2D _enemy_textures[8];
        Texture2D _weapon_textures[WEAPON_COUNT];
        Texture2D _potion_textures[POTION_COUNT];
        Texture2D _shop_goblin_texture;
        Texture2D _stickman_texture;

        // music
        int _current_bgm = 0;
        Music _bgm[7];
        Sound _wave_horn_sfx;
        Sound _get_coin_sfx[3];
        float _enemy_scale[8] = {1.5f, 2.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f};

        // 虛擬畫布
        bool _should_quit = false;
        RenderTexture2D _canvas;
        float _canvas_scale = 1.0f;
        float _canvas_offset_x = 0;
        float _canvas_offset_y = 0;
        // damage text
        std::vector<damage_text> _damage_text;

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


// 好用的函式
float get_distance(enemy* a, enemy* b);