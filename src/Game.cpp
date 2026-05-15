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
    // 移除飛出畫面的子彈
    for(int i = _projectiles.size() - 1; i >= 0; i--){
        Vector2 pos = _projectiles[i].get_position();
        if(pos.x < 0 || pos.x > 2400 || pos.y < 0 || pos.y > 900){
            _projectiles.erase(_projectiles.begin() + i);
        }
    }
    for(int i = _projectiles.size()-1 ; i>=0 ; i--){ // projectile, enemy 
        for(int j = _enemies.size()-1 ; j>=0 ; j--){
            if(CheckCollisionRecs(_projectiles[i].get_rect(),_enemies[j].get_rect())){
                _enemies[j].take_damage(_projectiles[i].get_damage());
                _projectiles.erase(_projectiles.begin() + i);
                break;
            }
        }
    }
    for(int i = _enemies.size()-1;i>=0;i--){      // castle, enemy
        if(CheckCollisionRecs(_enemies[i].get_rect(),_castle.get_rect())){
            _castle.take_damage(_enemies[i].get_hp());
            _enemies.erase(_enemies.begin() + i);
        }
    }
    for(int i = _enemies.size()-1;i>=0;i--){      // player, enemy
        if(CheckCollisionRecs(_enemies[i].get_rect(),_player.get_rect())){
            _player.take_damage(_enemies[i].get_hp());
            _enemies.erase(_enemies.begin() + i);
        }
    }

    for(int i = _enemies.size()-1 ; i>=0 ; i--){ // check enemy is dead
        if(_enemies[i].get_hp()<=0){
            _enemies.erase(_enemies.begin() + i);
        }
    }


    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && _player.is_attackable()){
        _player.reset_attack_timer();
        Vector2 mouse_pos = GetMousePosition();
        Vector2 player_pos = _player.get_position();
        Vector2 delta = {mouse_pos.x - player_pos.x, mouse_pos.y - player_pos.y};
        float length = sqrt(delta.x * delta.x + delta.y * delta.y);
        if(length > 0){
            float bullet_speed = 1000.0;
            Vector2 speed = {(delta.x/length)*bullet_speed,(delta.y/length)*bullet_speed};
            projectile p = game_factory::create_projectile(player_pos, speed);
            _projectiles.push_back(p);
        }
    }

}
void game::draw(){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangleRec(_player.get_rect(), BLUE);
    DrawRectangleRec(_castle.get_rect(), GRAY);
    for(int i = 0;i<_enemies.size();i++){
        DrawRectangleRec(_enemies[i].get_rect(), RED);
    }
    for(int i = 0;i<_projectiles.size();i++){
        DrawRectangleRec(_projectiles[i].get_rect(), BLACK);
    }
    EndDrawing();
}
void game::run(){
    while(WindowShouldClose() == false){
        update(GetFrameTime());
        draw();
    }
}