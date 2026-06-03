#include "Game.h"
void game::run(){
    while(WindowShouldClose() == false && !_should_quit){
        // 音樂
        if(IsMusicStreamPlaying(_bgm[_current_bgm]) == false){
            int temp;
            do{
                temp = GetRandomValue(0, 6);
            }while(temp == _current_bgm);
            _current_bgm = temp;
            PlayMusicStream(_bgm[_current_bgm]);
        }
        UpdateMusicStream(_bgm[_current_bgm]);
        
        _canvas_scale = fmin((float)GetScreenWidth()/2400.0f, (float)GetScreenHeight()/900.0f);
        _canvas_offset_x = (GetScreenWidth() - 2400*_canvas_scale) / 2;
        _canvas_offset_y = (GetScreenHeight() - 900*_canvas_scale) / 2;
        
        BeginTextureMode(_canvas);
        ClearBackground(RAYWHITE);
        if(_game_statement == START){   
            handle_start();
        }else if(_game_statement == PLAYING){
            handle_playing();
        }else if(_game_statement == TUTORIAL){        
            handle_tutorial();
        }else if(_game_statement == PAUSE){ 
            handle_pause();
        }else if(_game_statement == WAVE_SHOP){
            handle_wave_shop();
        }else if(_game_statement == LOSE || _game_statement == WIN){ 
            handle_end();
        }
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(_canvas.texture, {0,0,2400,-900}, {_canvas_offset_x, _canvas_offset_y, 2400*_canvas_scale, 900*_canvas_scale}, {0,0}, 0, WHITE);
        EndDrawing();
    }
}


game::game() : _player(game_factory::create_player({640, game_factory::GROUND_Y-30})), _castle(game_factory::create_castle({-10, game_factory::GROUND_Y-568})){}

void game::init(){
    // 開window, 載入圖片
    InitWindow(2400, 900, "Tower Defense Game");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);  // 取消 ESC 關閉視窗
    SetTargetFPS(60);
    _enemy_textures[SLIMEGREEN] = LoadTexture("resources/object/monster/monster_green.png");
    _enemy_textures[SLIMEBLACK] = LoadTexture("resources/object/monster/monster_black.png");
    _enemy_textures[SLIMERED] = LoadTexture("resources/object/monster/monster_red.png");
    _enemy_textures[SLIMEPURPLE] = LoadTexture("resources/object/monster/monster_purple.png");
    _enemy_textures[SLIMEBLUE] = LoadTexture("resources/object/monster/monster_blue.png");
    _enemy_textures[FLYINGANGEL] = LoadTexture("resources/object/monster/angel_2.png");
    _enemy_textures[FLYINGBIRD] = LoadTexture("resources/object/monster/bird_24.png");
    _enemy_textures[FLYINGDRAGON] = LoadTexture("resources/object/monster/dragon_2.png");
    _enemy_textures[FLYINGWIND] = LoadTexture("resources/object/monster/angel_1.png");
    _castle_texture = LoadTexture("resources/object/castle.png");
    _background_texture = LoadTexture("resources/background/background.png");
    _player_texture = LoadTexture("resources/object/player_archer.png");
    _coin_texture = LoadTexture("resources/object/coin/coin_4.png");
    for(int i = 0; i < WEAPON_COUNT; i++) _weapon_textures[i] = LoadTexture(TextFormat("resources/object/weapon/weapon_%d.png", i));
    for(int i = 0; i < POTION_COUNT; i++) _potion_textures[i] = LoadTexture(TextFormat("resources/object/potion/potion_%d.png", i));
    _shop_goblin_texture = LoadTexture("resources/object/shop/goblin.png");
    _stickman_texture = LoadTexture("resources/object/shop/stickman.png");
    _victory_crown_texture = LoadTexture("resources/object/special/crown.png");
    _special_gift_texture = LoadTexture("resources/object/special/gift.png");
    _buff_textures[BUFF_ATTACK] = LoadTexture("resources/effect/atk_buff.png");
    _buff_textures[BUFF_ATTACK_SPEED] = LoadTexture("resources/effect/atk_buff.png");
    _buff_textures[BUFF_MOVE_SPEED] = LoadTexture("resources/effect/move_buff.png");
    _buff_textures[BUFF_REGEN] = LoadTexture("resources/effect/regen_buff.png");
    _buff_textures[BUFF_SHIELD] = LoadTexture("resources/effect/shield_buff.png");
    _buff_textures[BUFF_CRIT_RATE] = LoadTexture("resources/effect/crit_rate_buff.png");
    _buff_textures[BUFF_CRIT_DAMAGE] = LoadTexture("resources/effect/crit_dmg_buff.png");
    _buff_textures[DEBUFF] = LoadTexture("resources/effect/debuff.png");
    // music
    
    InitAudioDevice();
    for(int i = 0; i < 7; i++){
        _bgm[i] = LoadMusicStream(TextFormat("resources/music/BGM/GameBoyBGM-%d.mp3", i+1));
        SetMusicVolume(_bgm[i], 0.8f);
    }
    _current_bgm = GetRandomValue(0, 6);
    PlayMusicStream(_bgm[_current_bgm]);
    SetMusicVolume(_bgm[_current_bgm], 0.8f);
    // sound 
    _wave_horn_sfx = LoadSound("resources/music/SoundEffect/wave-horn/wave-horn.mp3");
    for(int i = 0;i<3;i++) _get_coin_sfx[i] = LoadSound(TextFormat("resources/music/SoundEffect/get-coin/get-coin%d.mp3",i+1));
    SetSoundVolume(_wave_horn_sfx, 0.5f);
    for(int i = 0;i<3;i++) SetSoundVolume(_get_coin_sfx[i], 0.5f);
    // canvas
    _canvas = LoadRenderTexture(2400, 900);
    // wave
    load_waves("resources/levels.txt");
}
void game::reset(){
    _player = game_factory::create_player({640, game_factory::GROUND_Y-30});
    _castle = game_factory::create_castle({-10, game_factory::GROUND_Y-568});
    for(int i = 0;i<_enemies.size();i++) delete _enemies[i];
    // game
    _game_statement = START;
    // player
    _player_damage = PLAYER_VALUES.damage;
    _player_crit_chance = 0.0f;
    _player_crit_multiplier = 0.0f;
    _multi_shot = 1;
    // enemy
    _enemies.clear();
    _enemy_spawn_timer = 0;
    _enemies_spawned = 0;
    _kill_count = 0;
    // projectiles
    _projectiles.clear();
    // wave
    _current_wave = 0;
    _is_wave_active = false;
    _wave_rest_timer = 0;
    // golds
    _coins.clear();
    _golds = PLAYER_VALUES.start_golds;
    _max_golds = PLAYER_VALUES.max_golds;
    // player_levels
    for(int i = 0;i<9;i++) _player_level[i] = 0;
    // announce
    _announce_wave_timer = 0;
    _is_announcing_wave = false;
    // weapon
    _current_weapon = MUD;
    // potion effects
    _buffs[BUFF_ATTACK] = {1.0f, 1.0f, 0};
    _buffs[BUFF_ATTACK_SPEED] = {1.0f, 1.0f, 0};
    _buffs[BUFF_SHIELD] = {1.0f, 1.0f, 0};
    _buffs[BUFF_MOVE_SPEED] = {1.0f, 1.0f, 0};
    _buffs[BUFF_REGEN] = {0, 0, 0};
    _buffs[BUFF_CRIT_RATE] = {0.0f, 0.0f, 0};
    _buffs[BUFF_CRIT_DAMAGE] = {1.0f, 1.0f, 0};
    // tutorial
    _tutorial_page = 0;
    // debug
    _debug_hitbox = false;
    // press delay
    _press_delay = 0;
    // shop
    if(_shop_goblin != nullptr){ delete _shop_goblin; _shop_goblin = nullptr; }
    // crown
    _is_crown_collected = false;
    if(_victory_crown != nullptr){ delete _victory_crown; _victory_crown = nullptr; }
    // gift
    if(_special_gift != nullptr) { delete _special_gift; _special_gift = nullptr; }

    // damage text
    _damage_text.clear();
    _shop_weapon1 = MUD;
    _shop_weapon2 = MUD;
    _shop_potion = HEAL_PLAYER_POTION;
}


// close 
void game::close(){
    for(int i = 0; i < 9; i++) UnloadTexture(_enemy_textures[i]);
    UnloadTexture(_castle_texture);
    UnloadTexture(_background_texture);
    UnloadTexture(_player_texture);
    UnloadTexture(_coin_texture);
    for(int i = 0;i<WEAPON_COUNT;i++) UnloadTexture(_weapon_textures[i]);
    for(int i = 0;i<POTION_COUNT;i++) UnloadTexture(_potion_textures[i]);
    UnloadTexture(_shop_goblin_texture);
    UnloadTexture(_stickman_texture);
    UnloadTexture(_victory_crown_texture);
    UnloadTexture(_special_gift_texture);
    for(int i = 0;i<BUFF_COUNT;i++) UnloadTexture(_buff_textures[i]);
    // music
    for(int i = 0;i<7;i++) UnloadMusicStream(_bgm[i]);
    // sound
    UnloadSound(_wave_horn_sfx);
    for(int i = 0;i<3;i++) UnloadSound(_get_coin_sfx[i]);
    
    CloseAudioDevice();
    UnloadRenderTexture(_canvas);
    CloseWindow();
}

// load waves from  "resources\levels.txt"
void game::load_waves(const char* path){
    std::map<std::string, enemy_type> string_to_type = {
        {"LGN", SLIMEGREEN},
        {"LBK", SLIMEBLACK},
        {"LRD", SLIMERED},
        {"LBE", SLIMEBLUE},
        {"LPE", SLIMEPURPLE},
        {"SAL", FLYINGANGEL},
        {"SBD", FLYINGBIRD},
        {"SDN", FLYINGDRAGON},
        {"SWD", FLYINGWIND}
    };
    std::ifstream file(path);
    std::string line;
    while(std::getline(file, line)){
        std::istringstream ss(line);  
        std::vector<wave_data> data; 
        float current_cooldown = 1.0f;
        float hp_multiplier = 1.0f;
        std::string token;
        while(ss >> token){
            if(token[0] == 'x'){
                hp_multiplier = std::stof(token.substr(1));
            }
            else if(isdigit(token[0])){
                current_cooldown = std::stof(token);  // stof string to float 
            }else{
                std::string name;
                int star = token.find('*');
                int count = 1;
                if(star!= std::string::npos){  // no position
                    name = token.substr(0,star);
                    count = std::stoi(token.substr(star+1)); // stoi string to int 
                }else{
                    name = token;
                }
                for(int i = 0;i<count;i++){
                    data.push_back({string_to_type[name],current_cooldown});
                }
            }
        }
        _waves.push_back(wave(data,hp_multiplier));
    }
}


// helper function 
float get_distance(enemy* a, enemy* b){
    float dx = a->get_position().x - b->get_position().x;
    float dy = a->get_position().y - b->get_position().y;
    return sqrt(dx*dx + dy*dy);
}

