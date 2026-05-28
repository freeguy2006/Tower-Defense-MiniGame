#include "Game.h"
// ------------------------------------  update ----------------------------------
void game::update(float dt){
    // 1. 輸入              --------------------------------------------
    // player shoot
    _player.set_attack_cooldown(_player.get_base_cooldown() * _buffs[BUFF_ATTACK_SPEED].value);
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && _player.is_attackable()){
        _player.reset_attack_timer();
        Vector2 mouse_raw = GetMousePosition();
        Vector2 mouse_pos = {(mouse_raw.x - _canvas_offset_x) / _canvas_scale, (mouse_raw.y - _canvas_offset_y) / _canvas_scale};
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
    if(_current_wave >= _waves.size() && _is_crown_collected == false){
        if(_victory_crown == nullptr){
            _victory_crown = new crown({2000,-100}, game_factory::GROUND_Y-68);
        }
        _victory_crown->update(dt);
        if(CheckCollisionRecs(_player.get_rect(), _victory_crown->get_rect())){
            _is_crown_collected = true;
            delete _victory_crown;
            _victory_crown = nullptr;
            _game_statement = WIN;
        }
    }else if(_current_wave < _waves.size()){
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
                    PlaySound(_wave_horn_sfx);
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
                
                float hpm = w.get_hp_multiplier();
                switch(type){
                    case SLIMEGREEN: _enemies.push_back(game_factory::create_enemy_green({2500, game_factory::GROUND_Y-54}, hpm)); break;
                    case SLIMEBLACK: _enemies.push_back(game_factory::create_enemy_black({2500, game_factory::GROUND_Y-88}, hpm)); break;
                    case SLIMERED: _enemies.push_back(game_factory::create_enemy_red({2500, game_factory::GROUND_Y-60}, hpm)); break;
                    case SLIMEPURPLE: _enemies.push_back(game_factory::create_enemy_purple({2500, game_factory::GROUND_Y-61}, hpm)); break;
                    case SLIMEBLUE: _enemies.push_back(game_factory::create_enemy_blue({2500, game_factory::GROUND_Y-54}, hpm)); break;
                    case FLYINGANGEL: _enemies.push_back(game_factory::create_enemy_angel({2500, game_factory::GROUND_Y-GetRandomValue(250,300)}, hpm)); break;
                    case FLYINGBIRD: _enemies.push_back(game_factory::create_enemy_bird({2500, game_factory::GROUND_Y-GetRandomValue(300,550)}, hpm)); break;
                    case FLYINGDRAGON: _enemies.push_back(game_factory::create_enemy_dragon({2500, game_factory::GROUND_Y-GetRandomValue(300,500)}, hpm)); break;
                    case FLYINGWIND: _enemies.push_back(game_factory::create_enemy_wind({(float)GetRandomValue(1000, 2200), -150.0f}, hpm)); break;
                }
                
                _enemies_spawned++;
                _enemy_spawn_timer = 0;
                
            }
            if(_enemies_spawned >= w.get_total_enemies() && _enemies.empty()){
                _is_wave_active = false;
                _current_wave++;
                _enemy_spawn_timer = 0;
                // 藥水效果倒數
                if(_buffs[BUFF_ATTACK].waves > 0){ 
                    _buffs[BUFF_ATTACK].waves--; 
                    if(_buffs[BUFF_ATTACK].waves == 0) _buffs[BUFF_ATTACK].value = 1.0f; 
                }
                if(_buffs[BUFF_ATTACK_SPEED].waves > 0){
                    _buffs[BUFF_ATTACK_SPEED].waves--; 
                    if(_buffs[BUFF_ATTACK_SPEED].waves == 0){ 
                        _buffs[BUFF_ATTACK_SPEED].value = 1.0f;
                        _player.set_attack_cooldown(_player.get_base_cooldown());
                    }
                }
                if(_buffs[BUFF_SHIELD].waves > 0){
                    _buffs[BUFF_SHIELD].waves--;
                    if(_buffs[BUFF_SHIELD].waves == 0){
                        _buffs[BUFF_SHIELD].value = 1.0f;
                    }
                }
                if(_buffs[BUFF_MOVE_SPEED].waves > 0){ 
                    _buffs[BUFF_MOVE_SPEED].waves--; 
                    if(_buffs[BUFF_MOVE_SPEED].waves == 0){
                        _buffs[BUFF_MOVE_SPEED].value = 1.0f;
                        _player.set_move_speed(_player.get_move_base_speed());
                    }
                }
                if(_buffs[BUFF_REGEN].waves > 0){ 
                    _buffs[BUFF_REGEN].waves--; 
                    if(_buffs[BUFF_REGEN].waves == 0) _buffs[BUFF_REGEN].value = 0; 
                }
                if(_buffs[BUFF_CRIT_RATE].waves>0){
                    _buffs[BUFF_CRIT_RATE].waves--;
                    if(_buffs[BUFF_CRIT_RATE].waves == 0){
                        _buffs[BUFF_CRIT_RATE].value = 0.0f;
                    }
                }
                if(_buffs[BUFF_CRIT_DAMAGE].waves>0){
                    _buffs[BUFF_CRIT_DAMAGE].waves--;
                    if(_buffs[BUFF_CRIT_DAMAGE].waves == 0){
                        _buffs[BUFF_CRIT_DAMAGE].value = 1.0f;
                    }
                }
                // 每三波進去shop
                if(_current_wave%3 == 0){
                    _shop_weapon1 = (weapon_type)GetRandomValue(0,WEAPON_COUNT-1); 
                    _shop_weapon2 = (weapon_type)GetRandomValue(0,WEAPON_COUNT-1); 
                    _shop_potion = (potion_type)GetRandomValue(2,POTION_COUNT-1); 
                    _shop_goblin = game_factory::create_goblin({2300, game_factory::GROUND_Y-96});
                }
            }
        }
    }
    // gift
    if(_is_gift_active == true && _special_gift == nullptr){
        _special_gift = game_factory::create_special_gift({1500, -100}, game_factory::GROUND_Y - 106);
        _is_gift_active = false;
    }
    
    // 3. 狀態更新              --------------------------------------------
    for(int i = 0;i<_enemies.size();i++){
        _enemies[i]->reset_speed();
        _enemies[i]->set_nearby_buff(false);
        _enemies[i]->set_nearby_heal(false);
    }
    for(const auto& i:_enemies){ 
        for(const auto& j:_enemies){
            if(i == j)continue;
            const auto& behaviors = j->get_behaviors();
            for(int k = 0;k<behaviors.size();k++){
                buff_behavior* buff = dynamic_cast<buff_behavior*>(behaviors[k]);
                if(buff){ // 加速，加傷害，抗性
                    float dist = get_distance(i,j);
                    if(dist <= buff->get_buff_range()){
                        Vector2 spd = i->get_speed();
                        spd.x *= buff->get_speed_boost();
                        i->set_speed(spd);
                        i->set_nearby_buff(true);
                    }
                }
                heal_behavior* heal = dynamic_cast<heal_behavior*>(behaviors[k]);
                if(heal && heal->is_able_to_heal()){ // 治療
                    float dist = get_distance(i,j);
                    if(dist <= heal->get_heal_range()){
                        i->heal(heal->get_heal_amount());
                        i->set_nearby_heal(true);
                    }
                }
            }
        }
    }
    for(int i = _damage_text.size()-1;i>=0;i--){
        _damage_text[i].timer += dt;
        _damage_text[i].position.y -= 50 * dt; 
        float alpha = 1.0f - (_damage_text[i].timer / _damage_text[i].lifetime);
        Color c = _damage_text[i].color;
        c.a = (unsigned char)(alpha * 255);
        _damage_text[i].color = c;
        if(_damage_text[i].timer >= _damage_text[i].lifetime){
            _damage_text.erase(_damage_text.begin() + i);
        }
    }

    _player.update(dt);
    _castle.update(dt);
    for(int i = 0;i<_enemies.size();i++)        _enemies[i]->update(dt);
    for(int i = 0;i<_projectiles.size();i++)    _projectiles[i].update(dt);
    for(int i = 0;i<_coins.size();i++)          _coins[i].update(dt);
    if(_shop_goblin != nullptr)                 _shop_goblin->update(dt);
    if(_special_gift != nullptr)                _special_gift->update(dt);
    // 4. 碰撞              --------------------------------------------
    // projectile, enemy
    for(int i = _projectiles.size()-1 ; i>=0 ; i--){ 
        for(int j = _enemies.size()-1 ; j>=0 ; j--){
            if(CheckCollisionRecs(_projectiles[i].get_rect(),_enemies[j]->get_rect())){
                float damage = _projectiles[i].get_damage();
                // 減傷區
                for(int k = 0;k<_enemies.size();k++){ 
                    if(k == j) continue;
                    const auto& behaviors = _enemies[k]->get_behaviors();
                    for(int b = 0;b<behaviors.size();b++){
                        // dynamic cast 把 enemy 轉成 buff ，jump 變 null
                        buff_behavior* buff = dynamic_cast<buff_behavior*>(behaviors[b]); 
                        if(buff != nullptr){
                            float dist = get_distance(_enemies[j],_enemies[k]);
                            if(dist <= buff->get_buff_range()){
                                damage *= buff->get_damage_reduction(); 
                            }
                        }
                    }
                }
                // 乘傷區
                float total_crit_chance = _projectiles[i].get_crit_chance() + _player_crit_chance;
                float total_crit_multiplier = _projectiles[i].get_crit_multiplier() + _player_crit_multiplier;
                if(_buffs[BUFF_CRIT_RATE].waves>0){
                    total_crit_chance += _buffs[BUFF_CRIT_RATE].value;
                }
                if(_buffs[BUFF_CRIT_DAMAGE].waves>0){
                    total_crit_multiplier += total_crit_multiplier * _buffs[BUFF_CRIT_DAMAGE].value;
                }
                bool is_crit = false;
                if(total_crit_chance > 0){
                    float roll = (float)GetRandomValue(1,10000)/10000.0f;
                    if(roll <= total_crit_chance){
                        damage = damage * total_crit_multiplier;
                        damage += _enemies[j]->get_hp() * _projectiles[i].get_crit_hp_percent();
                        is_crit = true;
                    }
                    
                }
                damage = damage * _buffs[BUFF_ATTACK].value;
                _enemies[j]->take_damage(damage);
                if(is_crit) _damage_text.push_back({{_enemies[j]->get_position().x+GetRandomValue(-20,20),_enemies[j]->get_position().y},TextFormat("crit! %d",(int)damage),0.0f,1.0f,ORANGE});
                else _damage_text.push_back({{_enemies[j]->get_position().x+GetRandomValue(-20,20),_enemies[j]->get_position().y},TextFormat("%d",(int)damage),0.0f,1.0f,RED});
                
                if(_projectiles[i].get_splash_range() > 0){
                    float splashdmg = damage * _projectiles[i].get_splash_damage();
                    for(int k = _enemies.size()-1;k>=0;k--){
                        if(k == j) continue;
                        float dist = get_distance(_enemies[j],_enemies[k]);
                        if(dist <= _projectiles[i].get_splash_range()){
                            _enemies[k]->take_damage(splashdmg);
                            _damage_text.push_back({{_enemies[k]->get_position().x+GetRandomValue(-20,20),_enemies[k]->get_position().y},TextFormat("Splash! %d",(int)splashdmg),0.0f,1.0f,RED});
                            if(_projectiles[i].get_slow_percent() > 0)
                                _enemies[k]->apply_slow(_projectiles[i].get_slow_percent(), _projectiles[i].get_slow_duration());
                            if(_projectiles[i].get_freeze_duration() > 0)
                                _enemies[k]->apply_freeze(_projectiles[i].get_freeze_duration());
                            if(_projectiles[i].get_poison_damage() > 0)
                                _enemies[k]->add_poison(_projectiles[i].get_poison_damage(), _projectiles[i].get_poison_interval());
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
                if(_projectiles[i].is_piercing()){
                    Vector2 pos = _projectiles[i].get_position();
                    Vector2 spd = _projectiles[i].get_speed();
                    float len = sqrt(spd.x * spd.x + spd.y * spd.y);
                    if(len > 0){
                        float skip = fmax(_enemies[j]->get_size().x, _enemies[j]->get_size().y);
                        pos.x += (spd.x / len) * skip;
                        pos.y += (spd.y / len) * skip;
                        _projectiles[i].set_position(pos);
                    }
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
            _castle.take_damage(damage*_buffs[BUFF_SHIELD].value);
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
            _player.take_damage(damage*_buffs[BUFF_SHIELD].value);
            delete _enemies[i];
            _enemies.erase(_enemies.begin() + i);
        }
    }
    // coin, player
    for(int i = _coins.size()-1; i >= 0; i--){
        if(CheckCollisionRecs(_player.get_rect(), _coins[i].get_rect())){
            PlaySound(_get_coin_sfx[GetRandomValue(0, 2)]);
            _golds += _coins[i].get_value();
            if(_golds > _max_golds) _golds = _max_golds;
            _coins.erase(_coins.begin() + i);
        }
    }
    // globin
    if(_shop_goblin != nullptr ){
        for(int i = _projectiles.size()-1;i>=0;i--){
            if(CheckCollisionRecs(_projectiles[i].get_rect(),_shop_goblin->get_rect())){
                _shop_goblin->take_damage(_projectiles[i].get_damage());
                _projectiles.erase(_projectiles.begin() + i);
            }
        }
        if(_shop_goblin->is_alive() && CheckCollisionRecs(_shop_goblin->get_rect(), _castle.get_rect())){
            _castle.heal(_castle.get_max_hp() * 0.3f);
            delete _shop_goblin;
            _shop_goblin = nullptr;
            
        }
        else if(_shop_goblin->is_alive() && CheckCollisionRecs(_shop_goblin->get_rect(), _player.get_rect())){
            _game_statement = WAVE_SHOP;
            delete _shop_goblin;
            _shop_goblin = nullptr;
        }
    }
    // player, gift
    if(_special_gift != nullptr && CheckCollisionRecs(_player.get_rect(),_special_gift->get_rect())){
        delete _special_gift;
        _special_gift = nullptr;
        _golds = 9999;
        _max_golds = 9999;
        _player.increase_max_hp(9999.0f - _player.get_max_hp());
        _player.heal(9999.0f);
        _castle.increase_max_hp(9999.0f - _castle.get_max_hp());
        _castle.heal(9999.0f);
        _player_damage += 200.0f;   
    }
    
    // 5. 效果              --------------------------------------------
    // player heal
    _player.heal(_buffs[BUFF_REGEN].value * dt);
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
    // goblin
    if(_shop_goblin != nullptr && !_shop_goblin->is_alive()){
        Vector2 pos = _shop_goblin->get_position();
        int coin_num = _shop_goblin->get_reward();
        for(int c = 0; c < coin_num; c++){
            _coins.push_back(coin(pos, 1, game_factory::GROUND_Y-32));
        }
        delete _shop_goblin;
        _shop_goblin = nullptr;
    }
    
    // enemy_poison
    for(int i = 0; i < _enemies.size(); i++){
        if(_enemies[i]->is_poison_ready()){
            _enemies[i]->set_poison_ready(false);
            float dmg = _enemies[i]->get_poison_tick_damage();
            _enemies[i]->take_damage(dmg);
            _enemies[i]->decay_poison_combo();
            _damage_text.push_back({_enemies[i]->get_position(), TextFormat("Poison! %d", (int)dmg), 0.0f, 1.0f, GREEN});
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
    // 虛擬畫布

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
            buff_behavior* buff = dynamic_cast<buff_behavior*>(behaviors[j]);
            if(buff){
                DrawCircleLines(center.x,center.y,buff->get_buff_range(),BLUE);
            }
            heal_behavior* heal = dynamic_cast<heal_behavior*>(behaviors[j]);
            if(heal){
                DrawCircleLines(center.x,center.y,heal->get_heal_range(),YELLOW);
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

// ----------------------------- run -------------------------------
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