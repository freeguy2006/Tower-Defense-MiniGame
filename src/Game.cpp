// game 主迴圈與邏輯，擁有 player, castle, vector<enemy>, vector<projectile>
// 自己的: update(float dt), draw(), run(), add_enemy(enemy)
// player 可用: update(), get_rect(), get_position(), is_attackable(), reset_attack_timer()
// castle 可用: get_rect(), get_hp(), is_alive(), take_damage(int)
// enemy 可用: update(), get_rect(), get_hp(), is_alive(), take_damage(int)
// projectile 可用: update(), get_rect(), get_damage(), get_position()
// factory 可用: game_factory::create_player(), create_enemy(), create_castle(), create_projectile()
#include "Game.h"

void game::update(float dt){
    _player.update(dt);
    _castle.update(dt);
    for(int i = 0;i<_enemies.size();i++){
        _enemies[i].update(dt);
    }
    for(int i = 0;i<_projectiles.size();i++){
        _projectiles[i].update(dt);
    }
    // enemy spawn
    _enemy_spawn_timer+=dt;
    if(_enemy_spawn_timer>=_enemy_spawn_cooldown){
        enemy e = game_factory::create_enemy({2300, 800});
        _enemies.push_back(e);
        _enemy_spawn_timer = 0;
    }
    // object collision ----------------------------------------------------
    // projectile, enemy 
    for(int i = _projectiles.size()-1 ; i>=0 ; i--){ 
        for(int j = _enemies.size()-1 ; j>=0 ; j--){
            if(CheckCollisionRecs(_projectiles[i].get_rect(),_enemies[j].get_rect())){
                _enemies[j].take_damage(_projectiles[i].get_damage());
                _projectiles.erase(_projectiles.begin() + i);
                break;
            }
        }
    }
    // castle, enemy
    for(int i = _enemies.size()-1;i>=0;i--){      
        if(CheckCollisionRecs(_enemies[i].get_rect(),_castle.get_rect())){
            _castle.take_damage(_enemies[i].get_hp());
            _enemies.erase(_enemies.begin() + i);
        }
    }
    // player, enemy
    for(int i = _enemies.size()-1;i>=0;i--){      
        if(CheckCollisionRecs(_enemies[i].get_rect(),_player.get_rect())){
            _player.take_damage(_enemies[i].get_hp());
            _enemies.erase(_enemies.begin() + i);
        }
    }
    //------------------------------------------------------------------------

    // check enemy is dead
    for(int i = _enemies.size()-1 ; i>=0 ; i--){ 
        if(_enemies[i].get_hp()<=0){
            _enemies.erase(_enemies.begin() + i);
            _kill_count++;
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
    if(_player.get_hp() == 0 || _castle.get_hp() == 0){
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
void game::draw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    //DrawTextureEx(_player_texture,_player.get_position(),0,1.5,WHITE);
    // castle
    DrawTextureEx(_castle_texture,_castle.get_position(),0,2,WHITE);
    // enemy
    for(int i = 0;i<_enemies.size();i++){
        DrawTextureEx(_enemy_texture,_enemies[i].get_position(),0,1.5,WHITE);
        DrawRectangleRec({_enemies[i].get_position().x, _enemies[i].get_position().y-5, (float)_enemies[i].get_size().x, 5}, GRAY);
        DrawRectangleRec({_enemies[i].get_position().x, _enemies[i].get_position().y-5, (float)_enemies[i].get_size().x * (float)_enemies[i].get_hp() / (float)_enemies[i].get_max_hp(), 5}, RED);
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
    //player
    DrawRectangleRec(_player.get_rect(), BLUE);

    // 血條
    DrawRectangleRec({20, 20, 200, 20}, GRAY);
    DrawRectangleRec({20, 20, 200.0f * _castle.get_hp() / _castle.get_max_hp(), 20}, RED);
    DrawText(TextFormat("Castle HP: %d",_castle.get_hp()),20,20,20,BLACK);
    
    DrawRectangleRec({20, 50, 200, 20}, GRAY);
    DrawRectangleRec({20, 50, 200.0f * _player.get_hp() / _player.get_max_hp(), 20}, ORANGE);
    DrawText(TextFormat("Player HP: %d",_player.get_hp()),20,50,20,BLACK);
    
    DrawText(TextFormat("Kills: %d",_kill_count),20,80,20,BLACK);
    EndDrawing();
}
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
        }else if(_game_statement == LOSE){
            //失敗畫面
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Game Over",200,400,100,DARKGRAY);
            DrawText("Press Enter to exit",200,600,70,DARKGRAY);
            EndDrawing();
            if(IsKeyPressed(KEY_ENTER)){
                break;
            } 
        }
    }
}