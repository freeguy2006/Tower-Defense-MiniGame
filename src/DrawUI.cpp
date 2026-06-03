#include "Game.h"
void game::draw(){
    // 先畫在canvas上，再畫到畫面
    // background
    DrawTexturePro(_background_texture,{0, 0, (float)_background_texture.width, (float)_background_texture.height},{0, 0, 3000,1080},{0, 0}, 0, WHITE);
    // castle
    DrawTextureEx(_castle_texture,_castle.get_position(),0,0.6,WHITE);
    // enemy
    for(int i = 0;i<_enemies.size();i++){
        Vector2 epos = _enemies[i]->get_position();
        Vector2 esize = _enemies[i]->get_size();
        int etype = _enemies[i]->get_enemy_type();
        DrawTextureEx(_enemy_textures[etype], epos, 0, _enemy_scale[etype], WHITE);
        // enemy 血條
        DrawRectangleRec({epos.x, epos.y-5, esize.x, 5}, GRAY);
        DrawRectangleRec({epos.x, epos.y-5, esize.x * _enemies[i]->get_hp() / _enemies[i]->get_max_hp(), 5}, RED);
        float icon_x = epos.x - 20;
        if(_enemies[i]->is_frozen() || _enemies[i]->is_slowed() || _enemies[i]->is_poisoned()){
            DrawTexturePro(_buff_textures[DEBUFF], {0,0,512,512}, {icon_x, epos.y-25, 20, 20}, {0,0}, 0, WHITE);
        }
        if(_enemies[i]->has_nearby_buff()){
            icon_x += 22;
            DrawTexturePro(_buff_textures[BUFF_ATTACK], {0,0,512,512}, {icon_x, epos.y-25, 20, 20}, {0,0}, 0, WHITE);
            icon_x += 22;
            DrawTexturePro(_buff_textures[BUFF_MOVE_SPEED], {0,0,512,512}, {icon_x, epos.y-25, 20, 20}, {0,0}, 0, WHITE);
            icon_x += 22;
            DrawTexturePro(_buff_textures[BUFF_SHIELD], {0,0,512,512}, {icon_x, epos.y-25, 20, 20}, {0,0}, 0, WHITE);
        }
        if(_enemies[i]->has_nearby_heal()){
            icon_x += 22;
            DrawTexturePro(_buff_textures[BUFF_REGEN], {0,0,512,512}, {icon_x, epos.y-25, 20, 20}, {0,0}, 0, WHITE);
        }
    }
    // buff, heal 光圈
    for(int i = 0;i<_enemies.size();i++){
        Vector2 epos = _enemies[i]->get_position();
        Vector2 esize = _enemies[i]->get_size();
        Vector2 center = {epos.x + esize.x/2, epos.y + esize.y/2};
        const auto& behaviors = _enemies[i]->get_behaviors();
        for(int j = 0;j < behaviors.size();j++){
            if(behaviors[j]->has_buff()){
                DrawCircleLines(center.x,center.y,behaviors[j]->get_buff_range(),BLUE);
            }
            if(behaviors[j]->has_heal()){
                DrawCircleLines(center.x,center.y,behaviors[j]->get_heal_range(),YELLOW);
            }
        }
    }
    // projectile
    for(int i = 0;i<_projectiles.size();i++){
        Vector2 spd = _projectiles[i].get_speed(); 
        float angle = atan2(spd.y, spd.x) * 180.0f / PI - 135;
        Vector2 pos = _projectiles[i].get_position();
        const Texture2D& tex = _weapon_textures[_projectiles[i].get_weapon_type()];
        float w = tex.width;
        float h = tex.height;
        Rectangle source = {0, 0, w, h};
        Rectangle dest = {pos.x, pos.y, w, h};
        Vector2 origin = {w/2, h/2};
        DrawTexturePro(tex, source, dest, origin, angle, WHITE);
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

    // wave_announce
    if(_is_announcing_wave){
        int blink = (int)(_announce_wave_timer * 3) % 2;
        if(blink == 0){
            DrawText(TextFormat("<<< wave %d",_current_wave+1), 1700, 400, 100 ,RED);
        }
    }
    //血條
    //castle
    DrawRectangleRec({18, 18, 1004, 29}, BLACK);
    DrawRectangleRec({20, 20, 1000, 25}, GRAY);
    DrawRectangleRec({20, 20, 1000.0f * _castle.get_hp() / _castle.get_max_hp(), 25}, RED);
    DrawText(TextFormat("Castle HP: %.0f",_castle.get_hp()),25,20,25,BLACK);
    //player
    DrawRectangleRec({18, 58, 504, 29}, BLACK);
    DrawRectangleRec({20, 60, 500, 25}, GRAY);
    DrawRectangleRec({20, 60, 500.0f * _player.get_hp() / _player.get_max_hp(), 25}, ORANGE);
    DrawText(TextFormat("Player HP: %.0f",_player.get_hp()),25,60,25,BLACK);
    int icon_x = 530;
    for(int i = 0; i < BUFF_COUNT; i++){
        if(_buffs[i].waves > 0){
            DrawTexturePro(_buff_textures[i], {0,0,512,512}, {(float)icon_x, 55, 30, 30}, {0,0}, 0, WHITE);
            icon_x += 35;
        }
    }
    // golds
    float gold_bar_width = 300.0f * _golds / _max_golds;
    DrawRectangleRec({2048, 18, 304, 34}, BLACK);
    DrawRectangleRec({2050, 20, 300, 30}, GRAY);
    DrawRectangleRec({2350 - gold_bar_width, 20, gold_bar_width, 30}, GOLD);
    DrawText(TextFormat("Gold: %d / %d", _golds, _max_golds), 2060, 20, 30, BLACK);
    // 殺敵數
    DrawText(TextFormat("Kills: %d",_kill_count),1200,20,40,BLACK);
    // 第幾波
    if(_current_wave < _waves.size()){
        DrawText(TextFormat("Wave: %d / %d", _current_wave+1 , (int)_waves.size()),1600,20,40,BLACK );
    }else{
        DrawText(TextFormat("Wave: %d / %d", (int)_waves.size() , (int)_waves.size()),1600,20,40,BLACK );
    }
    // 倍率
    float hpm = 1.0f;
    if(_current_wave < _waves.size()){
        hpm = _waves[_current_wave].get_hp_multiplier();
    }else if(!_waves.empty()){
        hpm = _waves.back().get_hp_multiplier();
    }
    float factor = (hpm - 1.0f) / 10.0f;
    if (factor < 0.0f) factor = 0.0f;
    if (factor > 1.0f) factor = 1.0f;
    unsigned char val = (unsigned char)(factor * 255.0f);
    Color x = { val, 0, 0, 255};
    DrawText(TextFormat("Enemy HP x%.1f", hpm), 1600, 70, 30, x);
    
    //_shop_goblin   
    if(_shop_goblin != nullptr)  DrawTextureEx(_shop_goblin_texture, _shop_goblin->get_position(), 0, 0.2f, WHITE);
    //_special_gift
    if(_special_gift != nullptr) DrawTextureEx(_special_gift_texture, _special_gift->get_position(), 0, 0.3f, WHITE);
    // coin
    for(int i = 0;i<_coins.size();i++) DrawTextureEx(_coin_texture,_coins[i].get_position(),0,1,WHITE);
    // crown
    if(_victory_crown != nullptr) DrawTextureEx(_victory_crown_texture, _victory_crown->get_position(), 0, 0.07f, WHITE);
    
    // damage text
    for(int i = 0;i<_damage_text.size();i++){
        DrawText(_damage_text[i].text.c_str(),_damage_text[i].position.x,_damage_text[i].position.y-20,30,_damage_text[i].color);
    }
    // debug hitbox
    if(_debug_hitbox){
        DrawRectangleLinesEx(_castle.get_rect(), 2, GREEN);
        DrawRectangleLinesEx(_player.get_rect(), 2, GREEN);
        for(int i = 0; i < _enemies.size(); i++) DrawRectangleLinesEx(_enemies[i]->get_rect(), 2, GREEN);
        for(int i = 0; i < _coins.size(); i++) DrawRectangleLinesEx(_coins[i].get_rect(), 2, GREEN);
        if(_shop_goblin != nullptr) DrawRectangleLinesEx(_shop_goblin->get_rect(), 2, GREEN);
        if(_victory_crown != nullptr) DrawRectangleLinesEx(_victory_crown->get_rect(), 2, GREEN);
        if(_special_gift != nullptr) DrawRectangleLinesEx(_special_gift->get_rect(), 2, GREEN);
    }
    
}

// 特殊 gamestatement 的介面
void game::handle_start(){
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
    if(IsKeyPressed(KEY_ENTER)){
        _game_statement = PLAYING;
    }
}
void game::handle_tutorial(){
    DrawText("~~Tutorial~~", 250, 50, 100, DARKGRAY);
    if(_tutorial_page == 0){
        DrawText("Press A D to move", 250, 300, 70, BLACK);
        DrawText("Press W to jump", 250, 400, 70, BLACK);
        DrawText("Press ESC to shop", 250, 500, 70, BLACK);
        DrawText("Press Enter to continue", 250, 700, 90, RED);
    }else if(_tutorial_page == 1){
        DrawText("Use mouse to aim and click to shoot", 250, 300, 70, BLACK);
        DrawText("You can tap ESC to shop", 250, 400, 70, BLACK);
        DrawText("Tap numbers to buy items", 250, 500, 70, BLACK);
        DrawText("Press Enter to continue !", 250, 700, 90, RED);
    }else if(_tutorial_page == 2){
        DrawText("there are 8 types of enemies", 250, 300, 70, BLACK);
        DrawText("Angels can heal enemies around them", 250, 400, 70, BLACK);
        DrawText("ICE SLIME can buff enemies around them", 250, 500, 70, BLACK);
        DrawText("Press Enter to continue !!", 250, 700, 90, RED);
    }else if(_tutorial_page == 3){
        DrawText("HOT tips :", 250, 200, 70, BLACK);
        DrawText("You have 250 golds at the beginning", 250, 300, 70, BLACK);
        DrawText("Goblin is rich and you can buy useful items from it", 250, 400, 70, BLACK);
        DrawText("You can design your own level in resources/levels.txt", 250, 500, 70, BLACK);
        DrawText("Press Enter to continue !!!", 250, 700, 90, RED);
    }else if(_tutorial_page == 4){
        DrawText("There are 11 types of weapon and 8 types of potion", 250, 300, 70, BLACK);
        DrawText("Good luck and have fun !", 250, 400, 70, BLACK);
        DrawText("Upgrade your abilities to protect the castle !", 250, 500, 70, BLACK);
        DrawText("Press ESC to exit !!!", 250, 700, 90, RED);
    }else if(_tutorial_page <= 50){
        DrawText("Didn't you watch my tutorial ?", 250, 300, 100, BLACK);
        DrawText("Press ESC to exit !!!", 250, 670, 120, RED);
    }else{
        _is_gift_active = true;
        DrawText("Congratulations!!! ", 250, 300, 100, RED);
        DrawText("You unlock a mystery gift !", 250, 500, 100, ORANGE);
        DrawText("Press ESC to exit !!!", 250, 700, 70, BLACK);
    }
    
    if(IsKeyPressed(KEY_ENTER)){
        _tutorial_page++;
        if(_tutorial_page>60) _tutorial_page = 60;
    }
    if(IsKeyPressed(KEY_ESCAPE)) _game_statement = START;
}
void game::handle_pause(){
    DrawText("~~ SHOP ~~",700,50,100,DARKGRAY);
    DrawTextureEx(_stickman_texture, {1500,270}, 0, 2.0f, WHITE);
    // your status
    weapon_type equiped_weapon = _current_weapon;
    DrawTextureEx(_weapon_textures[equiped_weapon], {1550,100}, 0, 1.0f, WHITE);
    DrawText(TextFormat("you equiped : %s", WEAPON_NAME[equiped_weapon]), 1600, 100, 40, BLACK);
    DrawText(TextFormat("you: %d / %d", (int)_player.get_hp(), (int)_player.get_max_hp()), 1600, 160, 40, _player.get_hp()<_player.get_max_hp()/3 ? RED : BLACK);
    DrawText(TextFormat("castle: %d / %d", (int)_castle.get_hp(), (int)_castle.get_max_hp()), 1600, 220, 40, _castle.get_hp()<_castle.get_max_hp()/3 ? RED : BLACK);

    int cost[9];
    for(int i = 0; i < 9; i++){
        cost[i] = UPGRADE_VALUES[i].cost_base + UPGRADE_VALUES[i].cost_gain * _player_level[i];
        DrawText(TextFormat("%s", UPGRADE_VALUES[i].name), 400, 250 + i*60, 40, _golds >= cost[i] ? BLUE : GRAY);
        DrawText(TextFormat("Lv: %d", _player_level[i]), 900, 250+i*60, 40, _golds >= cost[i] ? BLUE : GRAY);
        DrawText(TextFormat("Cost: %d", cost[i]), 1100, 250+i*60, 40, _golds >= cost[i] ? BLUE : GRAY);
    }
    DrawText(TextFormat("Gold: %d / %d", _golds, _max_golds), 1500, 690, 60, GOLD);
    DrawText("Press ESC to continue", 1500, 760, 40, DARKGRAY);
    if(IsKeyPressed(KEY_ONE) && _golds >= cost[0]){
        _golds -= cost[0]; _player_damage += 5.0f; _player_level[0]++;
    }
    if(IsKeyPressed(KEY_TWO) && _golds >= cost[1]){
        _golds -= cost[1]; _player.increase_max_hp(100); _player_level[1]++;
    }
    if(IsKeyPressed(KEY_THREE) && _golds >= cost[2]){
        _golds -= cost[2]; _castle.increase_max_hp(100); _player_level[2]++;
    }
    if(IsKeyPressed(KEY_FOUR) && _golds >= cost[3]){
        _golds -= cost[3]; _max_golds += 100; _player_level[3]++;
    }
    if(IsKeyPressed(KEY_FIVE) && _golds >= cost[4]){
        _golds -= cost[4]; _player.decrease_cooldown(0.02f); _player_level[4]++;
    }
    if(IsKeyPressed(KEY_SIX) && _golds >= cost[5]){
        _golds -= cost[5]; _multi_shot += 1; _player_level[5]++;
    }
    if(IsKeyPressed(KEY_SEVEN) && _golds >= cost[6]){
        _golds -= cost[6]; _player.increase_move_speed(30); _player_level[6]++;
    }
    if(IsKeyPressed(KEY_EIGHT) && _golds >= cost[7]){
        _golds -= cost[7]; _player_crit_chance += 0.02f; _player_level[7]++;
    }
    if(IsKeyPressed(KEY_NINE) && _golds >= cost[8]){
        _golds -= cost[8]; _player_crit_multiplier += 0.25f; _player_level[8]++;
    }
    // pause to playing
    _press_delay -= GetFrameTime();
    if(IsKeyPressed(KEY_ESCAPE) && _press_delay <= 0){
        _game_statement = PLAYING;
        _press_delay =  0.2;
    }
}
void game::handle_wave_shop(){
    // you owned 
    weapon_type equiped_weapon = _current_weapon;
    DrawTextureEx(_weapon_textures[equiped_weapon], {1550,150}, 0, 1.0f, WHITE);
    DrawText(TextFormat("you equiped : %s", WEAPON_NAME[equiped_weapon]), 1600, 150, 40, BLACK);
    DrawText(TextFormat("you: %d / %d", (int)_player.get_hp(), (int)_player.get_max_hp()), 1600, 210, 40, _player.get_hp()<_player.get_max_hp()/3 ? RED : BLACK);
    DrawText(TextFormat("castle: %d / %d", (int)_castle.get_hp(), (int)_castle.get_max_hp()), 1600, 270, 40, _castle.get_hp()<_castle.get_max_hp()/3 ? RED : BLACK);
    
    // title
    DrawTextureEx(_shop_goblin_texture, {1600,400}, 0, 1.0f, WHITE);
    DrawText("~~Wave Shop~~",640,50,100,GREEN);
    DrawText(TextFormat("Gold: %d / %d", _golds, _max_golds), 600, 690, 60, GOLD);
    DrawText("Press ESC to skip", 600, 760, 40, DARKGRAY);
    // 1
    DrawTextureEx(_potion_textures[0], {600, 250}, 0, 1.0f, WHITE);
    DrawText(TextFormat("[1] %s", POTION_NAME[0]), 650, 250, 40, _golds >= POTION_COST[0] ? BLUE : GRAY);
    DrawText(TextFormat("Cost: %d", POTION_COST[0]), 1150, 250, 40, _golds >= POTION_COST[0] ? BLUE : GRAY);
    // 2
    DrawTextureEx(_potion_textures[1], {600, 330}, 0, 1.0f, WHITE);
    DrawText(TextFormat("[2] %s", POTION_NAME[1]), 650, 330, 40, _golds >= POTION_COST[1] ? BLUE : GRAY);
    DrawText(TextFormat("Cost: %d", POTION_COST[1]), 1150, 330, 40, _golds >= POTION_COST[1] ? BLUE : GRAY);
    // 3
    DrawTextureEx(_weapon_textures[_shop_weapon1], {600, 410}, 0, 1.0f, WHITE);
    DrawText(TextFormat("[3] %s", WEAPON_NAME[_shop_weapon1]), 650, 410, 40, _golds >= WEAPON_COST[_shop_weapon1] ? BLUE : GRAY);
    if(equiped_weapon != _shop_weapon1) DrawText(TextFormat("Cost: %d", WEAPON_COST[_shop_weapon1]), 1150, 410, 40, _golds >= WEAPON_COST[_shop_weapon1] ? BLUE : GRAY);
    else DrawText(TextFormat("Equiped"), 1150, 410, 40, GRAY);
    // 4
    DrawTextureEx(_weapon_textures[_shop_weapon2], {600, 490}, 0, 1.0f, WHITE);
    DrawText(TextFormat("[4] %s", WEAPON_NAME[_shop_weapon2]), 650, 490, 40, _golds >= WEAPON_COST[_shop_weapon2] ? BLUE : GRAY);
    if(equiped_weapon != _shop_weapon2) DrawText(TextFormat("Cost: %d", WEAPON_COST[_shop_weapon2]), 1150, 490, 40, _golds >= WEAPON_COST[_shop_weapon2] ? BLUE : GRAY);
    else DrawText(TextFormat("Equiped"), 1150, 490, 40, GRAY);
    // 5 
    DrawTextureEx(_potion_textures[_shop_potion], {600, 570}, 0, 1.0f, WHITE);
    DrawText(TextFormat("[5] %s", POTION_NAME[_shop_potion]), 650, 570, 40, _golds >= POTION_COST[_shop_potion] ? BLUE : GRAY);
    DrawText(TextFormat("Cost: %d", POTION_COST[_shop_potion]), 1150, 570, 40, _golds >= POTION_COST[_shop_potion] ? BLUE : GRAY);
    
    if(IsKeyPressed(KEY_ONE) && _golds >= POTION_COST[HEAL_PLAYER_POTION]){
        _golds -= POTION_COST[HEAL_PLAYER_POTION];
        _player.heal(_player.get_max_hp());
    }
    if(IsKeyPressed(KEY_TWO) && _golds >= POTION_COST[HEAL_CASTLE_POTION]){
        _golds -= POTION_COST[HEAL_CASTLE_POTION];
        _castle.heal(_castle.get_max_hp());
    }
    if(IsKeyPressed(KEY_THREE) && _golds >= WEAPON_COST[_shop_weapon1] && _current_weapon != _shop_weapon1){
        _golds -= WEAPON_COST[_shop_weapon1];
        _current_weapon = _shop_weapon1;
    }
    if(IsKeyPressed(KEY_FOUR) && _golds >= WEAPON_COST[_shop_weapon2] && _current_weapon != _shop_weapon2){
        _golds -= WEAPON_COST[_shop_weapon2];
        _current_weapon = _shop_weapon2;
    }
    if(IsKeyPressed(KEY_FIVE) && _golds >= POTION_COST[_shop_potion]){
        _golds -= POTION_COST[_shop_potion];
        switch(_shop_potion){
            case ATTACK_POTION:
                _buffs[BUFF_ATTACK].value = 1.3f;
                _buffs[BUFF_ATTACK].waves += 10;
                break;
            case ATTACK_SPEED_POTION:
                _buffs[BUFF_ATTACK_SPEED].value = 0.7f;
                _buffs[BUFF_ATTACK_SPEED].waves += 10;
                break;
            case SHIELD_POTION:
                _buffs[BUFF_SHIELD].value = 0.3f;  // 受傷 -70%
                _buffs[BUFF_SHIELD].waves += 10;
                break;
            case MOVE_SPEED_POTION:
                _buffs[BUFF_MOVE_SPEED].value = 1.3f;
                _buffs[BUFF_MOVE_SPEED].waves += 10;
                _player.set_move_speed(_player.get_move_base_speed() * _buffs[BUFF_MOVE_SPEED].value);
                break;
            case REGENERATION_POTION:
                _buffs[BUFF_REGEN].value = _player.get_max_hp()*0.01f;
                _buffs[BUFF_REGEN].waves += 6;
                break;
            case CRIT_POTION:
                _buffs[BUFF_CRIT_RATE].value = 0.2f;
                _buffs[BUFF_CRIT_RATE].waves += 15;
                _buffs[BUFF_CRIT_DAMAGE].value = 0.5f;
                _buffs[BUFF_CRIT_DAMAGE].waves += 10;
                break;
            default:
                break;
        }
    }
    if(IsKeyPressed(KEY_ESCAPE)){
        _game_statement = PLAYING;
        _press_delay = 0.2f;
    }
}
void game::handle_end(){
    if(_game_statement == LOSE) DrawText("Game Over",200,400,100,DARKGRAY);
    else DrawText("VICTORY!",200,300,150,YELLOW);
    DrawText(TextFormat("You killed %d enemies in %d waves", _kill_count, _current_wave),1000,400,70,BLACK);
    DrawText("Press Enter to restart",200,600,70,DARKGRAY);
    DrawText("Press Q to exit",200,700,70,DARKGRAY);
    // 回饋表單
    DrawText("Press F for feedback", 200, 800, 50, DARKGRAY);
    if(IsKeyPressed(KEY_F)){
        OpenURL("https://docs.google.com/forms/d/e/1FAIpQLSfff2i4hcdbG2zlOw0zknkJNo24P6YkZn85cvGuxLnKJ1VuGg/viewform?usp=publish-editor");
    }

    if(IsKeyPressed(KEY_ENTER)){
        reset();
    }
    if(IsKeyPressed(KEY_Q)){
        _should_quit = true;
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