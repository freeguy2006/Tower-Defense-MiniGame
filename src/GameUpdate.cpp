#include "Game.h"
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
                    if(_shop_goblin != nullptr){ delete _shop_goblin; _shop_goblin = nullptr; }
                    _shop_goblin = game_factory::create_goblin({2300, game_factory::GROUND_Y-96}, _waves[_current_wave-1].get_hp_multiplier());
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
                if(behaviors[k]->has_buff()){
                    float dist = get_distance(i,j);
                    if(dist <= behaviors[k]->get_buff_range()){
                        Vector2 spd = i->get_speed();
                        spd.x *= behaviors[k]->get_speed_boost();
                        i->set_speed(spd);
                        i->set_nearby_buff(true);
                    }
                }
                if(behaviors[k]->has_heal() && behaviors[k]->is_able_to_heal()){
                    float dist = get_distance(i,j);
                    if(dist <= behaviors[k]->get_heal_range()){
                        i->heal(behaviors[k]->get_heal_amount());
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
                        if(behaviors[b]->has_buff()){
                            float dist = get_distance(_enemies[j],_enemies[k]);
                            if(dist <= behaviors[b]->get_buff_range()){
                                damage *= behaviors[b]->get_damage_reduction(); 
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
                    if(_enemies[j]->get_behaviors()[b]->has_buff()){
                        float dist = get_distance(_enemies[i],_enemies[j]);
                        if(dist <= _enemies[j]->get_behaviors()[b]->get_buff_range()){
                            damage *= _enemies[j]->get_behaviors()[b]->get_damage_boost();
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
                    if(_enemies[j]->get_behaviors()[b]->has_buff()){
                        float dist = get_distance(_enemies[i],_enemies[j]);
                        if(dist <= _enemies[j]->get_behaviors()[b]->get_buff_range()){
                            damage *= _enemies[j]->get_behaviors()[b]->get_damage_boost();
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
