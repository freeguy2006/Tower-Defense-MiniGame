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
    _player.update(dt);
    _castle.update(dt);

    // buff 
    for(int i = 0;i<_enemies.size();i++){
        _enemies[i]->reset_speed();
    }
    for(int i = 0;i<_enemies.size();i++){ 
        for(int j = 0;j<_enemies.size();j++){
            if(i==j) continue;
            for(int k = 0;k<_enemies[j]->get_behaviors().size();k++){
                buff_behavior* buff = dynamic_cast<buff_behavior*>(_enemies[j]->get_behaviors()[k]);
                if(buff){ // 加速，加傷害，抗性
                    float dx = _enemies[i]->get_position().x - _enemies[j]->get_position().x;
                    float dy = _enemies[i]->get_position().y - _enemies[j]->get_position().y;
                    float dist = sqrt(dx*dx+dy*dy);
                    if(dist <= buff->get_buff_range()){
                        Vector2 spd = _enemies[i]->get_speed();
                        spd.x *= buff->get_speed_boost();
                        _enemies[i]->set_speed(spd);
                    }
                }
                heal_behavior* heal = dynamic_cast<heal_behavior*>(_enemies[j]->get_behaviors()[k]);
                if(heal && heal->is_able_to_heal()){ // 治療
                    float dx = _enemies[i]->get_position().x - _enemies[j]->get_position().x;
                    float dy = _enemies[i]->get_position().y - _enemies[j]->get_position().y;
                    float dist = sqrt(dx*dx + dy*dy);
                    if(dist <= heal->get_heal_range()){
                        _enemies[i]->heal(heal->get_heal_amount());
                    }
                }
            }
        }
    }

    // position update
    for(int i = 0;i<_enemies.size();i++){
        _enemies[i]->update(dt);
    }
    for(int i = 0;i<_projectiles.size();i++){
        _projectiles[i].update(dt);
    }
    // enemy spawn
    if(_current_wave >= _waves.size()){
        _game_statement = WIN;
    }else{
        if(_is_wave_active == false){
            // 中場休息
            _wave_rest_timer+=dt;
            if(_wave_rest_timer>=_wave_rest_duration){
                _is_wave_active = true;
                _enemies_spawned = 0;
                _wave_rest_timer = 0;
            }
        }else{
            // 幹架
            wave& w = _waves[_current_wave]; // reference
            _enemy_spawn_timer += dt;
            if(_enemy_spawn_timer >= w.get_spawn_cooldown() && _enemies_spawned < w.get_total_enemies()){
                enemy_type type = w.get_coming_enemies()[_enemies_spawned];
                
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
            }
        }
    }
    // coin
    for(int i = 0;i<_coins.size();i++){
        _coins[i].update(dt);
    }
    // -----------------object collision ----------------------- 
    // projectile, enemy 
    for(int i = _projectiles.size()-1 ; i>=0 ; i--){ 
        for(int j = _enemies.size()-1 ; j>=0 ; j--){
            if(CheckCollisionRecs(_projectiles[i].get_rect(),_enemies[j]->get_rect())){
                int damage = _projectiles[i].get_damage();
                // 所有enemy找一遍
                for(int k = 0;k<_enemies.size();k++){ 
                    if(k == j) continue;

                    // 各個 enemies 的所有 behavior 搜一遍
                    for(int b = 0;b<_enemies[k]->get_behaviors().size();b++){
                        // dynamic cast 把 enemy 轉成 buff ，jump 變 null
                        buff_behavior* buff = dynamic_cast<buff_behavior*>(_enemies[k]->get_behaviors()[b]); 
                        // 如果 behavior 是 buff  
                        if(buff != nullptr){
                            // 算距離
                            float dx = _enemies[j]->get_position().x-_enemies[k]->get_position().x;
                            float dy = _enemies[j]->get_position().y-_enemies[k]->get_position().y;
                            float dist = sqrt(dx*dx+dy*dy);
                            if(dist <= buff->get_buff_range()){
                                damage *= buff->get_damage_reduction(); 
                            }
                        }
                    }
                }
                
                _enemies[j]->take_damage(damage);
                _projectiles.erase(_projectiles.begin() + i);
                break;
            }
        }
    }
    // castle, enemy
    for(int i = _enemies.size()-1;i>=0;i--){      
        if(CheckCollisionRecs(_enemies[i]->get_rect(),_castle.get_rect())){
            int damage = _enemies[i]->get_hp();
            for(int k = 0; k < _enemies.size(); k++){
                if(k == i) continue;
                for(int b = 0; b < _enemies[k]->get_behaviors().size(); b++){
                    buff_behavior* buff = dynamic_cast<buff_behavior*>(_enemies[k]->get_behaviors()[b]);
                    if(buff){
                        float dx = _enemies[i]->get_position().x - _enemies[k]->get_position().x;
                        float dy = _enemies[i]->get_position().y - _enemies[k]->get_position().y;
                        float dist = sqrt(dx*dx + dy*dy);
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
            int damage = _enemies[i]->get_hp();
            for(int k = 0; k < _enemies.size(); k++){
                if(k == i) continue;
                for(int b = 0; b < _enemies[k]->get_behaviors().size(); b++){
                    buff_behavior* buff = dynamic_cast<buff_behavior*>(_enemies[k]->get_behaviors()[b]);
                    if(buff){
                        float dx = _enemies[i]->get_position().x - _enemies[k]->get_position().x;
                        float dy = _enemies[i]->get_position().y - _enemies[k]->get_position().y;
                        float dist = sqrt(dx*dx + dy*dy);
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
    //------------------------------------------------------

    // check enemy is dead
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
    // _game_statement
    if(_player.is_alive()==false || _castle.is_alive()==false){
        _game_statement = LOSE;
    }
    
    // game pause
    if(IsKeyPressed(KEY_ESCAPE)){
        _game_statement = PAUSE;
    }
    // player shoot
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && _player.is_attackable()){
        _player.reset_attack_timer();
        Vector2 mouse_pos = GetMousePosition();
        Vector2 player_pos = _player.get_position();
        Vector2 player_center = {
            player_pos.x + _player.get_size().x / 2,
            player_pos.y + _player.get_size().y / 2
        };
        Vector2 delta = {mouse_pos.x - player_center.x, mouse_pos.y - player_center.y};
        float length = sqrt(delta.x * delta.x + delta.y * delta.y);
        if(length > 0){
            float bullet_speed = 1400.0;
            Vector2 speed = {(delta.x/length)*bullet_speed,(delta.y/length)*bullet_speed};
            
            projectile p = game_factory::create_projectile(player_center, speed);
            _projectiles.push_back(p);
        }
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
    #ifdef DEBUG_HITBOX
        DrawRectangleLinesEx(_castle.get_rect(), 2, GREEN);
    #endif
    // enemy
    for(int i = 0;i<_enemies.size();i++){
        switch(_enemies[i]->get_enemy_type()){
            case SLIMEGREEN: DrawTextureEx(_enemy_green_texture,_enemies[i]->get_position(),0,1.5,WHITE); break;
            case SLIMEBLACK: DrawTextureEx(_enemy_black_texture,_enemies[i]->get_position(),0,2,WHITE); break;
            case SLIMERED: DrawTextureEx(_enemy_red_texture,_enemies[i]->get_position(),0,1.5,WHITE); break;
            case SLIMEPURPLE: DrawTextureEx(_enemy_purple_texture,_enemies[i]->get_position(),0,1.5,WHITE); break;
            case SLIMEBLUE: DrawTextureEx(_enemy_blue_texture,_enemies[i]->get_position(),0,1.5,WHITE); break;
            case FLYINGANGEL: DrawTextureEx(_enemy_angel_texture,_enemies[i]->get_position(),0,1.5,WHITE); break;
            case FLYINGBIRD: DrawTextureEx(_enemy_bird_texture,_enemies[i]->get_position(),0,1.5,WHITE); break;
            case FLYINGDRAGON: DrawTextureEx(_enemy_dragon_texture,_enemies[i]->get_position(),0,1.5,WHITE); break;
        }
        #ifdef DEBUG_HITBOX 
                DrawRectangleLinesEx(_enemies[i]->get_rect(), 2, GREEN);
        #endif
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
        float w = _projectile_texture.width;
        float h = _projectile_texture.height;
        Rectangle source = {0, 0, w, h};
        Rectangle dest = {pos.x, pos.y, w, h};
        Vector2 origin = {w/2, h/2};
        DrawTexturePro(_projectile_texture, source, dest, origin, angle, WHITE);
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
    #ifdef DEBUG_HITBOX 
        DrawRectangleLinesEx(_player.get_rect(), 2, GREEN);
    #endif

    // coin
    for(int i = 0;i<_coins.size();i++){
        DrawTextureEx(_coin_texture,_coins[i].get_position(),0,1,WHITE);
    }
    // 血條
    DrawRectangleRec({20, 20, 1000, 25}, GRAY);
    DrawRectangleRec({20, 20, 1000.0f * _castle.get_hp() / _castle.get_max_hp(), 25}, RED);
    DrawText(TextFormat("Castle HP: %d",_castle.get_hp()),20,20,25,BLACK);
    
    DrawRectangleRec({20, 60, 500, 25}, GRAY);
    DrawRectangleRec({20, 60, 500.0f * _player.get_hp() / _player.get_max_hp(), 25}, ORANGE);
    
    // golds
    DrawRectangleRec({2100, 20, 250, 25}, GRAY);
    DrawRectangleRec({2100, 20, 250.0f * _golds / _max_golds, 25}, GOLD);
    DrawText(TextFormat("Gold: %d / %d", _golds, _max_golds), 2100, 20, 25, BLACK);
    DrawText(TextFormat("Player HP: %d",_player.get_hp()),20,60,25,BLACK);
    // 殺敵數
    DrawText(TextFormat("Kills: %d",_kill_count),1100,20,40,BLACK);
    // 第幾波
    DrawText(TextFormat("Wave: %d / %d", _current_wave+1 , _waves.size()),1300,20,40,BLACK );
    EndDrawing();
}

// ----------------------------- run -------------------------------
void game::run(){
    while(WindowShouldClose() == false){
        if(_game_statement == START){
            //開始畫面
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Tower Defense Game",200,400,100,DARKGRAY);
            DrawText("Press Enter to start",200,600,70,DARKGRAY);
            EndDrawing();
            if(IsKeyPressed(KEY_ENTER)){
                _game_statement = PLAYING;
            }
        }else if(_game_statement == PLAYING){
            update(GetFrameTime());
            draw();
        }else if(_game_statement == PAUSE){
            //暫停畫面
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Paused",200,400,100,DARKGRAY);
            DrawText("Press Enter to continue",200,600,70,DARKGRAY);
            EndDrawing();
            if(IsKeyPressed(KEY_ENTER)){
                _game_statement = PLAYING;
            }
        }else if(_game_statement == LOSE || _game_statement == WIN){
            //失敗畫面
            BeginDrawing();
            ClearBackground(RAYWHITE);
            if(_game_statement == LOSE) DrawText("Game Over",200,400,100,DARKGRAY);
            else DrawText("VICTORY!",200,400,100,YELLOW);
            DrawText("Press Enter to restart",200,600,70,DARKGRAY);
            DrawText("Press Q to exit",200,700,70,DARKGRAY);
            EndDrawing();
            if(IsKeyPressed(KEY_ENTER)){
                reset();
            }
            if(IsKeyPressed(KEY_Q)){
                break;
            }
        }
    }
}


game::game() : _player(game_factory::create_player({640, game_factory::GROUND_Y-30})), _castle(game_factory::create_castle({-10, game_factory::GROUND_Y-568})){}

void game::reset(){
    _player = game_factory::create_player({640, game_factory::GROUND_Y-30});
    _castle = game_factory::create_castle({100, game_factory::GROUND_Y-568});
    // _wave = 0;
    _kill_count = 0;
    for(int i = 0;i<_enemies.size();i++){
        delete _enemies[i];
    }
    _enemies.clear();
    _projectiles.clear();
    _game_statement = START;
    _enemy_spawn_timer = 0;
    _current_wave = 0;
    _enemies_spawned = 0;
    _is_wave_active = false;
    _wave_rest_timer = 0;
    _coins.clear();
    _golds = 0;
    _max_golds = 100;
}

// init 
void game::init(){
    // 開window, 載入圖片
    InitWindow(2400, 900, "Tower Defense Game");
    SetExitKey(0);  // 取消 ESC 關閉視窗
    SetTargetFPS(60);
    _enemy_green_texture = LoadTexture("resources/monster/slime/monster_green.png");
    _enemy_black_texture = LoadTexture("resources/monster/slime/monster_black.png");
    _enemy_red_texture = LoadTexture("resources/monster/slime/monster_red.png");
    _enemy_purple_texture = LoadTexture("resources/monster/slime/monster_purple.png");
    _enemy_blue_texture = LoadTexture("resources/monster/slime/monster_blue.png");
    _enemy_angel_texture = LoadTexture("resources/monster/angel/angel_2.png");
    _enemy_bird_texture = LoadTexture("resources/monster/bird/bird_24.png");
    _enemy_dragon_texture = LoadTexture("resources/monster/dragon/dragon_2.png");
    _projectile_texture = LoadTexture("resources/ammo/ammo_3.png");
    _castle_texture = LoadTexture("resources/castle/castle.png");
    _background_texture = LoadTexture("resources/background/background.png");
    _player_texture = LoadTexture("resources/player/player_archer.png");
    _coin_texture = LoadTexture("resources/coin/coin_4.png");
    // wave
    load_waves("resources/levels.txt");
}


// close 
void game::close(){
    UnloadTexture(_enemy_green_texture);
    UnloadTexture(_enemy_black_texture);
    UnloadTexture(_enemy_red_texture);
    UnloadTexture(_enemy_purple_texture);
    UnloadTexture(_enemy_blue_texture);
    UnloadTexture(_enemy_angel_texture);
    UnloadTexture(_enemy_bird_texture);
    UnloadTexture(_enemy_dragon_texture);
    UnloadTexture(_projectile_texture);
    UnloadTexture(_castle_texture);
    UnloadTexture(_background_texture);
    UnloadTexture(_player_texture);
    UnloadTexture(_coin_texture);
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
    while(std::getline(file,line)){
        std::istringstream ss(line);  
        float cooldown;
        ss >> cooldown;
        std::vector<enemy_type> enemies;
        std::string enemy_str;
        while(ss>>enemy_str){
            enemies.push_back(string_to_type[enemy_str]);
        }
        _waves.push_back(wave(enemies,cooldown));
    }
}