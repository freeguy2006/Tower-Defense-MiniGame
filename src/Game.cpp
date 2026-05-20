// game 主迴圈與邏輯，擁有 player, castle, vector<enemy>, vector<projectile>
// 自己的: update(float dt), draw(), run(), add_enemy(enemy)
// player 可用: update(), get_rect(), get_position(), is_attackable(), reset_attack_timer()
// castle 可用: get_rect(), get_hp(), is_alive(), take_damage(int)
// enemy 可用: update(), get_rect(), get_hp(), is_alive(), take_damage(int)
// projectile 可用: update(), get_rect(), get_damage(), get_position()
// factory 可用: game_factory::create_player(), create_enemy(), create_castle(), create_projectile()
#include "Game.h"
// ------------------------------------  update ----------------------------------
void game::update(float dt){
    // 1. 輸入              --------------------------------------------
    // player shoot
    _player.set_attack_cooldown(_player.get_base_cooldown() * _potion_attack_speed.value);
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && _player.is_attackable()){
        _player.reset_attack_timer();
        Vector2 mouse_pos = GetMousePosition();
        Vector2 player_center = {
            _player.get_position().x + _player.get_size().x / 2,
            _player.get_position().y + _player.get_size().y / 2
        };
        Vector2 delta = {mouse_pos.x - player_center.x, mouse_pos.y - player_center.y};
        float length = sqrt(delta.x * delta.x + delta.y * delta.y);

        if(length > 0){
            float bullet_speed = 1400.0;
            float spread = 0.1;
            for(int i = 0;i<_multi_shot;i++){
                float offset = (i - (_multi_shot - 1)/2.0f) *spread;
                Vector2 dir = {delta.x/length, delta.y/length};
                Vector2 speed = {(float)(dir.x*cos(offset)-dir.y*sin(offset))*bullet_speed,(float)(dir.x*sin(offset)+dir.y*cos(offset))*bullet_speed };
                projectile p = game_factory::create_projectile(player_center, _player_damage, speed);
                game_factory::apply_weapon(p, _current_weapon);
                _projectiles.push_back(p);
            }
        }
    }
    // 2. 生成              --------------------------------------------
    if(_current_wave >= _waves.size()){
        _game_statement = WIN;
    }else{
        if(_is_wave_active == false){
            // 中場休息
            if(_is_announcing_wave){
                _announce_wave_timer += dt; // 提示wave開始 2秒
                if(_announce_wave_timer >= _announce_wave_duration){
                    _is_announcing_wave = false;
                    _is_wave_active = true;
                    _enemies_spawned = 0;
                }
            }else{
                _wave_rest_timer += dt; // 休息 5秒 
                if(_wave_rest_timer >= _wave_rest_duration){
                    _is_announcing_wave = true;
                    _announce_wave_timer = 0;
                    _wave_rest_timer = 0;
                }
            }
        }else{
            // 幹架
            wave& w = _waves[_current_wave]; // reference
            _enemy_spawn_timer += dt;
            if( _enemies_spawned < w.get_total_enemies() && _enemy_spawn_timer >= w.get_spawn_cooldown(_enemies_spawned)){
                enemy_type type = w.get_type(_enemies_spawned);
                
                switch(type){
                    case SLIMEGREEN: _enemies.push_back(game_factory::create_enemy_green({2300, game_factory::GROUND_Y-54})); break;
                    case SLIMEBLACK: _enemies.push_back(game_factory::create_enemy_black({2300, game_factory::GROUND_Y-88})); break;
                    case SLIMERED: _enemies.push_back(game_factory::create_enemy_red({2300, game_factory::GROUND_Y-60})); break;
                    case SLIMEPURPLE: _enemies.push_back(game_factory::create_enemy_purple({2300, game_factory::GROUND_Y-61})); break;
                    case SLIMEBLUE: _enemies.push_back(game_factory::create_enemy_blue({2300, game_factory::GROUND_Y-54})); break;
                    case FLYINGANGEL: _enemies.push_back(game_factory::create_enemy_angel({2300, game_factory::GROUND_Y-GetRandomValue(250,300)})); break;
                    case FLYINGBIRD: _enemies.push_back(game_factory::create_enemy_bird({2300, game_factory::GROUND_Y-GetRandomValue(300,550)})); break;
                    case FLYINGDRAGON: _enemies.push_back(game_factory::create_enemy_dragon({2300, game_factory::GROUND_Y-GetRandomValue(300,500)})); break;
                }
                
                _enemies_spawned++;
                _enemy_spawn_timer = 0;
                
            }
            if(_enemies_spawned >= w.get_total_enemies() && _enemies.empty()){
                _is_wave_active = false;
                _current_wave++;
                _enemy_spawn_timer = 0;
                // 藥水效果倒數
                if(_potion_attack.waves > 0){ 
                    _potion_attack.waves--; 
                    if(_potion_attack.waves == 0) _potion_attack.value = 1.0f; 
                }
                if(_potion_attack_speed.waves > 0){
                    _potion_attack_speed.waves--; 
                    if(_potion_attack_speed.waves == 0){ 
                        _potion_attack_speed.value = 1.0f;
                        _player.set_attack_cooldown(_player.get_base_cooldown());
                    }
                }
                if(_potion_move_speed.waves > 0){ 
                    _potion_move_speed.waves--; 
                    if(_potion_move_speed.waves == 0){
                        _potion_move_speed.value = 1.0f;
                        _player.set_move_speed(_player.get_move_base_speed());
                    }
                }
                if(_potion_regen.waves > 0){ 
                    _potion_regen.waves--; 
                    if(_potion_regen.waves == 0) _potion_regen.value = 0; 
                }
                // 每三波進去shop
                if(_current_wave%3 == 0){
                    _shop_weapon1 = (weapon_type)GetRandomValue(0,WEAPON_COUNT-1); 
                    _shop_weapon2 = (weapon_type)GetRandomValue(0,WEAPON_COUNT-1); 
                    _shop_potion = (potion_type)GetRandomValue(2,POTION_COUNT-1); 
                    _game_statement = WAVE_SHOP;
                }
            }
        }
    }
    // 3. 狀態更新              --------------------------------------------
    for(int i = 0;i<_enemies.size();i++){
        _enemies[i]->reset_speed();
    }
    for(int i = 0;i<_enemies.size();i++){ 
        for(int j = 0;j<_enemies.size();j++){
            if(i==j) continue;
            for(int k = 0;k<_enemies[j]->get_behaviors().size();k++){
                buff_behavior* buff = dynamic_cast<buff_behavior*>(_enemies[j]->get_behaviors()[k]);
                if(buff){ // 加速，加傷害，抗性
                    float dist = get_distance(_enemies[i],_enemies[j]);
                    if(dist <= buff->get_buff_range()){
                        Vector2 spd = _enemies[i]->get_speed();
                        spd.x *= buff->get_speed_boost();
                        _enemies[i]->set_speed(spd);
                    }
                }
                heal_behavior* heal = dynamic_cast<heal_behavior*>(_enemies[j]->get_behaviors()[k]);
                if(heal && heal->is_able_to_heal()){ // 治療
                    float dist = get_distance(_enemies[i],_enemies[j]);
                    if(dist <= heal->get_heal_range()){
                        _enemies[i]->heal(heal->get_heal_amount());
                    }
                }
            }
        }
    }
    _player.update(dt);
    _castle.update(dt);
    for(int i = 0;i<_enemies.size();i++){
        _enemies[i]->update(dt);
    }
    for(int i = 0;i<_projectiles.size();i++){
        _projectiles[i].update(dt);
    }
    for(int i = 0;i<_coins.size();i++){
        _coins[i].update(dt);
    }
    // 4. 碰撞              --------------------------------------------
    // projectile, enemy 
    for(int i = _projectiles.size()-1 ; i>=0 ; i--){ 
        for(int j = _enemies.size()-1 ; j>=0 ; j--){
            if(CheckCollisionRecs(_projectiles[i].get_rect(),_enemies[j]->get_rect())){
                float damage = _projectiles[i].get_damage();
                // 減傷區
                for(int k = 0;k<_enemies.size();k++){ 
                    if(k == j) continue;
                    for(int b = 0;b<_enemies[k]->get_behaviors().size();b++){
                        // dynamic cast 把 enemy 轉成 buff ，jump 變 null
                        buff_behavior* buff = dynamic_cast<buff_behavior*>(_enemies[k]->get_behaviors()[b]); 
                        if(buff != nullptr){
                            float dist = get_distance(_enemies[j],_enemies[k]);
                            if(dist <= buff->get_buff_range()){
                                damage *= buff->get_damage_reduction(); 
                            }
                        }
                    }
                }
                // 乘傷區
                if(_projectiles[i].get_crit_chance() > 0){
                    float roll = (float)GetRandomValue(0,100)/100.0f;
                    if(roll <= _projectiles[i].get_crit_chance()){
                        damage = damage * _projectiles[i].get_crit_multiplier() + _projectiles[i].get_crit_damage();
                    }
                }
                if(_projectiles[i].get_crit_hp_percent()>0){
                    damage += _enemies[j]->get_hp() * _projectiles[i].get_crit_hp_percent();
                }
                damage = damage * _potion_attack.value;
                _enemies[j]->take_damage(damage);
                if(_projectiles[i].get_splash_range() > 0){
                    for(int k = _enemies.size()-1;k>=0;k--){
                        if(k == j) continue;
                        float dist = get_distance(_enemies[j],_enemies[k]);
                        if(dist <= _projectiles[i].get_splash_range()){
                            _enemies[k]->take_damage(_projectiles[i].get_splash_damage());
                            
                        }
                    }
                }
                if(_projectiles[i].get_slow_percent() > 0){
                    _enemies[j]->apply_slow(_projectiles[i].get_slow_percent(), _projectiles[i].get_slow_duration());
                }
                if(_projectiles[i].get_freeze_duration() > 0){
                    _enemies[j]->apply_freeze(_projectiles[i].get_freeze_duration());
                }
                if(_projectiles[i].get_poison_damage() > 0){
                    _enemies[j]->add_poison(_projectiles[i].get_poison_damage(),_projectiles[i].get_poison_interval());
                }
                if(_projectiles[i].is_piercing() == false){
                    _projectiles.erase(_projectiles.begin() + i);
                    break;
                }
            }
        }
    }
    // castle, enemy
    for(int i = _enemies.size()-1;i>=0;i--){      
        if(CheckCollisionRecs(_enemies[i]->get_rect(),_castle.get_rect())){
            float damage = _enemies[i]->get_hp();
            for(int j = 0; j < _enemies.size(); j++){
                if(j == i) continue;
                for(int b = 0; b < _enemies[j]->get_behaviors().size(); b++){
                    buff_behavior* buff = dynamic_cast<buff_behavior*>(_enemies[j]->get_behaviors()[b]);
                    if(buff){
                        float dist = get_distance(_enemies[i],_enemies[j]);
                        if(dist <= buff->get_buff_range()){
                            damage *= buff->get_damage_boost();
                        }
                    }
                }
            }
            _castle.take_damage(damage);
            delete _enemies[i];
            _enemies.erase(_enemies.begin() + i);
        }
    }
    // player, enemy
    for(int i = _enemies.size()-1;i>=0;i--){      
        if(CheckCollisionRecs(_enemies[i]->get_rect(),_player.get_rect())){
            float damage = _enemies[i]->get_hp();
            for(int j = 0; j < _enemies.size(); j++){
                if(j == i) continue;
                for(int b = 0; b < _enemies[j]->get_behaviors().size(); b++){
                    buff_behavior* buff = dynamic_cast<buff_behavior*>(_enemies[j]->get_behaviors()[b]);
                    if(buff){
                        float dist = get_distance(_enemies[i],_enemies[j]);
                        if(dist <= buff->get_buff_range()){
                            damage *= buff->get_damage_boost();
                        }
                    }
                }
            }
            _player.take_damage(damage);
            delete _enemies[i];
            _enemies.erase(_enemies.begin() + i);
        }
    }
    // coin, player
    for(int i = _coins.size()-1; i >= 0; i--){
        if(CheckCollisionRecs(_player.get_rect(), _coins[i].get_rect())){
            _golds += _coins[i].get_value();
            if(_golds > _max_golds) _golds = _max_golds;
            _coins.erase(_coins.begin() + i);
        }
    }
    // 5. 效果              --------------------------------------------
    // player heal
    _player.heal(_potion_regen.value * dt);
    for(int i = _enemies.size()-1 ; i>=0 ; i--){ 
        if(_enemies[i]->get_hp()<=0){
            Vector2 pos = _enemies[i]->get_position();
            int coin_num = _enemies[i]->get_reward();
            for(int c = 0; c < coin_num; c++){
                _coins.push_back(coin(pos, 1, game_factory::GROUND_Y-32));
            }
            delete _enemies[i];
            _enemies.erase(_enemies.begin() + i);
            _kill_count++;
        }
    }
    
    // 6. 清理              --------------------------------------------

    // delete expired coin
    for(int i = _coins.size()-1; i >= 0; i--){
        if(_coins[i].is_expired()){
            _coins.erase(_coins.begin() + i);
        }
    }
    // delete projectiles out of window
    for(int i = _projectiles.size() - 1; i >= 0; i--){
        Vector2 pos = _projectiles[i].get_position();
        if(pos.x < 0 || pos.x > 2400 || pos.y < 0 || pos.y > 900){
            _projectiles.erase(_projectiles.begin() + i);
        }
    }



    // 7. 遊戲判定              --------------------------------------------
    if(_player.is_alive()==false || _castle.is_alive()==false){
        _game_statement = LOSE;
    }
    

}

 //------------------------- draw ---------------------------------
void game::draw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // background
    DrawTexturePro(_background_texture,{0, 0, (float)_background_texture.width, (float)_background_texture.height},{0, 0, 3000,1080},{0, 0}, 0, WHITE);
    // castle
    DrawTextureEx(_castle_texture,_castle.get_position(),0,0.6,WHITE);
    if(_debug_hitbox) DrawRectangleLinesEx(_castle.get_rect(), 2, GREEN);
    // enemy
    for(int i = 0;i<_enemies.size();i++){
        DrawTextureEx(_enemy_textures[_enemies[i]->get_enemy_type()], _enemies[i]->get_position(), 0, _enemy_scale[_enemies[i]->get_enemy_type()], WHITE);
        if(_debug_hitbox) DrawRectangleLinesEx(_enemies[i]->get_rect(), 2, GREEN);
        // enemy 血條
        DrawRectangleRec({_enemies[i]->get_position().x, _enemies[i]->get_position().y-5, (float)_enemies[i]->get_size().x, 5}, GRAY);
        DrawRectangleRec({_enemies[i]->get_position().x, _enemies[i]->get_position().y-5, (float)_enemies[i]->get_size().x * (float)_enemies[i]->get_hp() / (float)_enemies[i]->get_max_hp(), 5}, RED);
    }
    // buff, heal 光圈
    for(int i = 0;i<_enemies.size();i++){
        for(int j = 0;j < _enemies[i]->get_behaviors().size();j++){
            buff_behavior* buff = dynamic_cast<buff_behavior*>(_enemies[i]->get_behaviors()[j]);
            if(buff){
                Vector2 center = {
                    _enemies[i]->get_position().x+_enemies[i]->get_size().x/2,
                    _enemies[i]->get_position().y+_enemies[i]->get_size().y/2
                };
                DrawCircleLines(center.x,center.y,buff->get_buff_range(),BLUE); 
            }
            heal_behavior* heal = dynamic_cast<heal_behavior*>(_enemies[i]->get_behaviors()[j]);
            if(heal){
                Vector2 center = {
                    _enemies[i]->get_position().x+_enemies[i]->get_size().x/2,
                    _enemies[i]->get_position().y+_enemies[i]->get_size().y/2
                };
                DrawCircleLines(center.x,center.y,heal->get_heal_range(),YELLOW); 
            }
        }
    }
    // projectile
    for(int i = 0;i<_projectiles.size();i++){
        Vector2 spd = _projectiles[i].get_speed(); 
        float angle = atan2(spd.y, spd.x) * 180.0f / PI - 135;
        Vector2 pos = _projectiles[i].get_position();
        float w = _weapon_textures[_projectiles[i].get_weapon_type()].width;
        float h = _weapon_textures[_projectiles[i].get_weapon_type()].height;
        Rectangle source = {0, 0, w, h};
        Rectangle dest = {pos.x, pos.y, w, h};
        Vector2 origin = {w/2, h/2};
        DrawTexturePro(_weapon_textures[_projectiles[i].get_weapon_type()], source, dest, origin, angle, WHITE);
    }
    // player
    float w = _player_texture.width;
    float h = _player_texture.height;
    Rectangle source;
    if(_player.is_facing_right()) source = {0, 0, w, h};
    else source = {0, 0, -w, h};
    Vector2 pos = _player.get_position();
    Rectangle dest = {pos.x, pos.y, (float)_player.get_size().x, (float)_player.get_size().y};
    DrawTexturePro(_player_texture, source, dest, {0,0}, 0, WHITE);
    if(_debug_hitbox) DrawRectangleLinesEx(_player.get_rect(), 2, GREEN);

    // wave_announce
    if(_is_announcing_wave){
        int blink = (int)(_announce_wave_timer * 3) % 2;
        if(blink == 0){
            DrawText(TextFormat("<<< wave %d",_current_wave+1), 1700, 400, 100 ,RED);
        }
    }
    

    // coin
    for(int i = 0;i<_coins.size();i++){
        DrawTextureEx(_coin_texture,_coins[i].get_position(),0,1,WHITE);
    if(_debug_hitbox) DrawRectangleLinesEx(_coins[i].get_rect(), 2, GREEN);
    }
    //血條
    //castle
    DrawRectangleRec({20, 20, 1000, 25}, GRAY);
    DrawRectangleRec({20, 20, 1000.0f * _castle.get_hp() / _castle.get_max_hp(), 25}, RED);
    DrawText(TextFormat("Castle HP: %.0f",_castle.get_hp()),20,20,25,BLACK);
    //player
    DrawRectangleRec({20, 60, 500, 25}, GRAY);
    DrawRectangleRec({20, 60, 500.0f * _player.get_hp() / _player.get_max_hp(), 25}, ORANGE);
    DrawText(TextFormat("Player HP: %.0f",_player.get_hp()),20,60,25,BLACK);
    // golds
    float gold_bar_width = 250.0f * _golds / _max_golds;
    DrawRectangleRec({2100, 20, 250, 30}, GRAY);
    DrawRectangleRec({2350 - gold_bar_width, 20, gold_bar_width, 30}, GOLD);
    DrawText(TextFormat("Gold: %d / %d", _golds, _max_golds), 2100, 20, 30, BLACK);
    // 殺敵數
    DrawText(TextFormat("Kills: %d",_kill_count),1100,20,40,BLACK);
    // 第幾波
    DrawText(TextFormat("Wave: %d / %d", _current_wave+1 , (int)_waves.size()),1300,20,40,BLACK );
    EndDrawing();
}

// ----------------------------- run -------------------------------
void game::run(){
    while(WindowShouldClose() == false){
        if(_game_statement == START){               // start
            handle_start();
        }else if(_game_statement == PLAYING){
            handle_playing();
        }else if(_game_statement == TUTORIAL){         // tutorial
            handle_tutorial();
        }else if(_game_statement == PAUSE){          // pause
            handle_pause();
        }else if(_game_statement == WAVE_SHOP){
            handle_wave_shop();
        }else if(_game_statement == LOSE || _game_statement == WIN){    // win, lose
            handle_end();
        }
    }
}


game::game() : _player(game_factory::create_player({640, game_factory::GROUND_Y-30})), _castle(game_factory::create_castle({-10, game_factory::GROUND_Y-568})){}

void game::reset(){
    _player = game_factory::create_player({640, game_factory::GROUND_Y-30});
    _castle = game_factory::create_castle({-10, game_factory::GROUND_Y-568});
    
    for(int i = 0;i<_enemies.size();i++){
        delete _enemies[i];
    }
    
    // game
    _game_statement = START;
    // player
    _player_damage = 2.0f;
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
    _golds = 0;
    _max_golds = 100;
    // player_levels
    for(int i = 0;i<7;i++){
        _player_level[i] = 0;
    }
    // announce
    _announce_wave_timer = 0;
    _is_announcing_wave = false;
    // weapon
    _current_weapon = MUD;
    // potion effects
    _potion_attack = {1.0f, 1.0f, 0};
    _potion_attack_speed = {1.0f, 1.0f, 0};
    _potion_move_speed = {1.0f, 1.0f, 0};
    _potion_regen = {0, 0, 0};
    // tutorial
    _tutorial_page = 0;
    // debug
    _debug_hitbox = false;
    // press delay
    _press_delay = 0;
}

// init 
void game::init(){
    // 開window, 載入圖片
    InitWindow(2400, 900, "Tower Defense Game");
    SetExitKey(0);  // 取消 ESC 關閉視窗
    SetTargetFPS(60);
    _enemy_textures[SLIMEGREEN] = LoadTexture("resources/monster/slime/monster_green.png");
    _enemy_textures[SLIMEBLACK] = LoadTexture("resources/monster/slime/monster_black.png");
    _enemy_textures[SLIMERED] = LoadTexture("resources/monster/slime/monster_red.png");
    _enemy_textures[SLIMEPURPLE] = LoadTexture("resources/monster/slime/monster_purple.png");
    _enemy_textures[SLIMEBLUE] = LoadTexture("resources/monster/slime/monster_blue.png");
    _enemy_textures[FLYINGANGEL] = LoadTexture("resources/monster/angel/angel_2.png");
    _enemy_textures[FLYINGBIRD] = LoadTexture("resources/monster/bird/bird_24.png");
    _enemy_textures[FLYINGDRAGON] = LoadTexture("resources/monster/dragon/dragon_2.png");
    _castle_texture = LoadTexture("resources/castle/castle.png");
    _background_texture = LoadTexture("resources/background/background.png");
    _player_texture = LoadTexture("resources/player/player_archer.png");
    _coin_texture = LoadTexture("resources/coin/coin_4.png");
    _weapon_textures[MUD] = LoadTexture("resources/ammo/weapon/ammo_1.png");
    _weapon_textures[ARROW] = LoadTexture("resources/ammo/weapon/ammo_3.png");
    _weapon_textures[STONE] = LoadTexture("resources/ammo/weapon/ammo_13.png");
    _weapon_textures[ICE_SLOW] = LoadTexture("resources/ammo/weapon/ammo_11.png");
    _weapon_textures[POISON] = LoadTexture("resources/ammo/weapon/ammo_18.png");
    _weapon_textures[PIERCING_ARROW] = LoadTexture("resources/ammo/weapon/ammo_17.png");
    _weapon_textures[IRON_BALL] = LoadTexture("resources/ammo/weapon/ammo_24.png");
    _weapon_textures[FIRE_BALL] = LoadTexture("resources/ammo/weapon/ammo_19.png");
    _weapon_textures[ROCKET] = LoadTexture("resources/ammo/weapon/ammo_5.png");
    _weapon_textures[ICE_FREEZE] = LoadTexture("resources/ammo/weapon/ammo_12.png");
    _weapon_textures[MISSILE] = LoadTexture("resources/ammo/weapon/ammo_26.png");
    // wave
    load_waves("resources/levels.txt");
}


// close 
void game::close(){
    for(int i = 0; i < 8; i++) UnloadTexture(_enemy_textures[i]);
    UnloadTexture(_castle_texture);
    UnloadTexture(_background_texture);
    UnloadTexture(_player_texture);
    UnloadTexture(_coin_texture);
    for(int i = 0;i<11;i++) UnloadTexture(_weapon_textures[i]);
    CloseWindow();
}

// load waves from  "resources\levels.txt"
void game::load_waves(const char* path){
    std::map<std::string, enemy_type> string_to_type = {
        {"SG", SLIMEGREEN},
        {"SK", SLIMEBLACK},
        {"SR", SLIMERED},
        {"SB", SLIMEBLUE},
        {"SP", SLIMEPURPLE},
        {"FA", FLYINGANGEL},
        {"FB", FLYINGBIRD},
        {"FD", FLYINGDRAGON}
    };
    std::ifstream file(path);
    std::string line;
    while(std::getline(file, line)){
        std::istringstream ss(line);  
        std::vector<wave_data> data; 
        float current_cooldown = 1.0f;
        std::string token;
        while(ss >> token){
            if(isdigit(token[0])){
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
        _waves.push_back(wave(data));
    }
}


// helper function 
float get_distance(enemy* a, enemy* b){
    float dx = a->get_position().x - b->get_position().x;
    float dy = a->get_position().y - b->get_position().y;
    return sqrt(dx*dx + dy*dy);
}




// 特殊 gamestatement 的介面
void game::handle_start(){
    //開始畫面
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Tower Defense Game",200,400,100,DARKGRAY);
    DrawText("Press Enter to start",200,600,70,DARKGRAY);
    // tutorial
    DrawText("Press G for tutorial", 200, 800, 40, DARKGRAY);
    // 回饋表單
    DrawText("Press F for feedback", 800, 800, 40, DARKGRAY);
    if(IsKeyPressed(KEY_G)){
        _game_statement = TUTORIAL;
        _tutorial_page = 0;
    }
    if(IsKeyPressed(KEY_F)){
        OpenURL("https://docs.google.com/forms/d/e/1FAIpQLSfff2i4hcdbG2zlOw0zknkJNo24P6YkZn85cvGuxLnKJ1VuGg/viewform?usp=publish-editor");
    }
    EndDrawing();
    if(IsKeyPressed(KEY_ENTER)){
        _game_statement = PLAYING;
    }
}
void game::handle_tutorial(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("~~Tutorial~~", 500, 50, 100, DARKGRAY);
    if(_tutorial_page == 0){
        DrawText("Press A D to move", 500, 250, 70, BLACK);
        DrawText("Press W to jump", 500, 350, 70, BLACK);
        DrawText("Press ESC to shop", 500, 450, 70, BLACK);
        DrawText("Press Enter to continue !", 500, 550, 90, RED);
    }else if(_tutorial_page == 1){
        DrawText("Use mouse to aim and click to shoot", 500, 250, 70, BLACK);
        DrawText("Tap numbers to buy items", 500, 350, 70, BLACK);
        DrawText("Press Enter to continue !!", 500, 450, 90, RED);
    }else if(_tutorial_page == 2){
        DrawText("there are 8 types of enemies", 500, 250, 70, BLACK);
        DrawText("Angels can heal enemies around them", 500, 350, 70, BLACK);
        DrawText("ICE SLIME can buff enemies around them", 500, 450, 70, BLACK);
        DrawText("Press ESC to exit !!!", 500, 550, 90, RED);
    }else if(_tutorial_page == 3){
        DrawText("Didn't you watch my tutorial ?", 500, 350, 100, BLACK);
        DrawText("Press ESC to exit !!!", 500, 550, 120, RED);
    }
    EndDrawing();
    if(IsKeyPressed(KEY_ENTER)){
        _tutorial_page++;
        if(_tutorial_page>3) _tutorial_page = 3;
    }
    if(IsKeyPressed(KEY_ESCAPE)){
        _game_statement = START;
    }
}
void game::handle_pause(){
    //暫停畫面
    BeginDrawing();
    ClearBackground(RAYWHITE);
    const char* item[7] = {"[1] Attack +1","[2] Player HP +10","[3] Castle HP +20","[4] Max Gold +50","[5] Attack Speed","[6] Multi Shot +1","[7] Move SpeedUp"};
    const int cost[7] = {20,20,20,40,50,80,40};
    const int cost_gain[7] = {10,10,10,30,30,80,20};
    DrawText("~~ SHOP ~~",200,50,100,DARKGRAY);
    for(int i = 0;i<7;i++){
        DrawText(TextFormat("%s",item[i]), 200, 250 + i*60, 40, _golds >= cost[i]+cost_gain[i]*_player_level[i] ? BLUE : GRAY);
        DrawText(TextFormat("Lv: %d", _player_level[i]), 700, 250+i*60, 40, _golds >= cost[i]+cost_gain[i]*_player_level[i] ? BLUE : GRAY);
        DrawText(TextFormat("Cost: %d",cost[i]+cost_gain[i]*_player_level[i]), 850, 250+i*60, 40, _golds >= cost[i]+cost_gain[i]*_player_level[i] ? BLUE : GRAY);
        
    }
    DrawText(TextFormat("Gold: %d / %d", _golds, _max_golds), 200, 700, 40, GOLD);
    DrawText("Press ESC to continue", 200, 760, 40, DARKGRAY);
    EndDrawing();

    if(IsKeyPressed(KEY_ONE) && _golds >= cost[0]+cost_gain[0]*_player_level[0]){
        _golds -= cost[0]+cost_gain[0]*_player_level[0]; _player_damage += 1.0f; _player_level[0]++;
    }
    if(IsKeyPressed(KEY_TWO) && _golds >= cost[1]+cost_gain[1]*_player_level[1]){
        _golds -= cost[1]+cost_gain[1]*_player_level[1]; _player.increase_max_hp(10); _player_level[1]++;
    }
    if(IsKeyPressed(KEY_THREE) && _golds >= cost[2]+cost_gain[2]*_player_level[2]){
        _golds -= cost[2]+cost_gain[2]*_player_level[2]; _castle.increase_max_hp(20); _player_level[2]++;
    }
    if(IsKeyPressed(KEY_FOUR) && _golds >= cost[3]+cost_gain[3]*_player_level[3]){
        _golds -= cost[3]+cost_gain[3]*_player_level[3]; _max_golds += 50; _player_level[3]++;
    }
    if(IsKeyPressed(KEY_FIVE) && _golds >= cost[4]+cost_gain[4]*_player_level[4]){
        _golds -= cost[4]+cost_gain[4]*_player_level[4]; _player.decrease_cooldown(0.05f); _player_level[4]++;
    }
    if(IsKeyPressed(KEY_SIX) && _golds >= cost[5]+cost_gain[5]*_player_level[5]){
        _golds -= cost[5]+cost_gain[5]*_player_level[5]; _multi_shot += 1; _player_level[5]++;
    }
    if(IsKeyPressed(KEY_SEVEN) && _golds >= cost[6]+cost_gain[6]*_player_level[6]){
        _golds -= cost[6]+cost_gain[6]*_player_level[6]; _player.increase_move_speed(10); _player_level[6]++;
    }
    // pause to playing
    _press_delay -= GetFrameTime();
    if(IsKeyPressed(KEY_ESCAPE) && _press_delay <= 0){
        _game_statement = PLAYING;
        _press_delay =  0.2;
    }
}
void game::handle_wave_shop(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("~~Wave Shop~~",200,50,100,DARKGRAY);
    DrawText("[1] Heal Player 50%", 200, 250, 40, BLUE);
    DrawText("[2] Heal Castle 50%", 200, 310, 40, BLUE);
    DrawText(TextFormat("[3] Weapon: %d", _shop_weapon1),200, 370, 40, BLUE);
    DrawText(TextFormat("[4] Weapon: %d", _shop_weapon2),200, 430, 40, BLUE);
    DrawText(TextFormat("[5] Potion: %d", _shop_potion),200, 490, 40, BLUE);
    DrawText("Press ESC to skip", 200, 600, 40, DARKGRAY);
    EndDrawing();
    
    if(IsKeyPressed(KEY_ONE)){
        _player.heal(_player.get_max_hp() * 0.5f);
        _game_statement = PLAYING;
    }
    if(IsKeyPressed(KEY_TWO)){
        _castle.heal(_castle.get_max_hp() * 0.5f);
        _game_statement = PLAYING;
    }
    if(IsKeyPressed(KEY_THREE)){
        _current_weapon = _shop_weapon1;
        _game_statement = PLAYING;
    }
    if(IsKeyPressed(KEY_FOUR)){
        _current_weapon = _shop_weapon2;
        _game_statement = PLAYING;
    }
    if(IsKeyPressed(KEY_FIVE)){
        switch(_shop_potion){
            case ATK_POTION:
                _potion_attack.value = 1.3f;
                _potion_attack.waves += 3;
                break;
            case ATKSPD_POTION:
                _potion_attack_speed.value = 0.7f;
                _potion_attack_speed.waves += 3;
                _player.set_attack_cooldown(_player.get_attack_cooldown() * _potion_attack_speed.value);
                break;
                case SHIELD_POTION:
                _player.increase_max_hp(_player.get_max_hp() * 0.5f);
                break;
            case MOVESPD_POTION:
                _potion_move_speed.value = 1.3f;
                _potion_move_speed.waves += 3;
                _player.set_move_speed(_player.get_move_base_speed() * _potion_move_speed.value);
                break;
            case REGEN_POTION:
                _potion_regen.value = _player.get_max_hp()*0.01f;
                _potion_regen.waves += 3;
                break;
            default:
                break;
        }
        _game_statement = PLAYING;
    }
    if(IsKeyPressed(KEY_ESCAPE)){
        _game_statement = PLAYING;
    }
}
void game::handle_end(){
    //失敗畫面
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if(_game_statement == LOSE) DrawText("Game Over",200,400,100,DARKGRAY);
    else DrawText("VICTORY!",200,400,100,YELLOW);
    DrawText("Press Enter to restart",200,600,70,DARKGRAY);
    DrawText("Press Q to exit",200,700,70,DARKGRAY);
    // 回饋表單
    DrawText("Press F for feedback", 200, 800, 50, DARKGRAY);
    if(IsKeyPressed(KEY_F)){
        OpenURL("https://docs.google.com/forms/d/e/1FAIpQLSfff2i4hcdbG2zlOw0zknkJNo24P6YkZn85cvGuxLnKJ1VuGg/viewform?usp=publish-editor");
    }

    EndDrawing();
    if(IsKeyPressed(KEY_ENTER)){
        reset();
    }
    if(IsKeyPressed(KEY_Q)){
        CloseWindow();
    }
}
void game::handle_playing(){
    _press_delay -= GetFrameTime();
    if(IsKeyPressed(KEY_ESCAPE) && _press_delay <= 0){
        _game_statement = PAUSE;
        _press_delay = 0.2;
    }else{
        if(IsKeyPressed(KEY_H)) _debug_hitbox = !_debug_hitbox;
        update(GetFrameTime());
        draw();
    }
}