// 敵人 (is-a character)，從右往左走到 target_x 停下
// 繼承可用: get_position(), set_position(), get_size(), is_active(), get_rect()
//          get_hp(), is_alive(), take_damage(float), get_speed(), set_speed()
#pragma once
#include "Character.h"
#include "EnemyBehavior.h"
#include <algorithm>
enum enemy_type {SLIMEGREEN,SLIMEBLACK,SLIMERED,SLIMEPURPLE,SLIMEBLUE,FLYINGANGEL,FLYINGBIRD,FLYINGDRAGON};
class enemy : public character{
    private:
        enemy_type _enemy_type;
        float _target_x;
        int _reward;
        Vector2 _base_speed;
        // enemy to enemy 效果
        std::vector<enemy_behavior*> _behaviors;
        // projectile to enemy 效果
        float _slow_timer = 0;
        float _slow_multiplier = 1.0f;
        float _freeze_timer = 0;
        int _poison_combo = 0;
        float _poison_damage = 0;
        float _poison_timer = 0;
        float _poison_interval = 1.0f;
        bool _poison_ready = false;
        bool _has_nearby_buff = false;
        bool _has_nearby_heal = false;
        

    protected: // 只會被自己或是flyingenemy用到
        bool update_status(float dt){ // 狀態更新
            if(_freeze_timer > 0){
                _freeze_timer -= dt;
                return true;
            }
            if(_slow_timer > 0){
                _slow_timer -= dt;
            }else{
                _slow_multiplier = 1.0f;
            }
            if(_poison_combo > 0){
                _poison_timer -= dt;
                if(_poison_timer < 0){ // 毒的效果，對 效果
                    _poison_ready = true;
                    _poison_timer += _poison_interval;
                }
            }
            return false; // 非冰凍狀態
        }
        float get_slow_multiplier() const { return _slow_multiplier; }
    public:
        enemy(Vector2 position, Vector2 size, bool active, float hp, Vector2 speed, float target_x, enemy_type type, int reward)
        : character(position, size, active, hp, speed), _target_x(target_x), _enemy_type(type), _base_speed(speed), _reward(reward){}
        virtual ~enemy(){
            for(int i = 0;i<_behaviors.size();i++){
                delete _behaviors[i];
            }
        };
        void add_behavior(enemy_behavior* behavior){ _behaviors.push_back(behavior); }
        const std::vector<enemy_behavior*>& get_behaviors() const { return _behaviors; }
        void reset_speed(){ set_speed(_base_speed); }
        // getter
        Vector2 get_base_speed() const { return _base_speed; }
        enemy_type get_enemy_type() const { return _enemy_type; }
        int get_reward() const { return _reward; }
        float get_target_x() const { return _target_x; }
        bool is_frozen() const { return _freeze_timer > 0; }
        bool is_slowed() const { return _slow_timer > 0; }
        bool is_poisoned() const { return _poison_combo > 0; }
        float get_poison_tick_damage() const { return _poison_damage * _poison_combo; }
        int get_poison_combo() const { return _poison_combo; }
        bool is_poison_ready(){ return _poison_ready; }        
        bool has_nearby_buff() const { return _has_nearby_buff; }
        bool has_nearby_heal() const { return _has_nearby_heal; }
        
        
        void apply_slow(float percent, float duration){
            _slow_multiplier = std::min(_slow_multiplier, 1.0f - percent);
            _slow_timer = std::max(_slow_timer, duration);
        }
        void apply_freeze(float duration){
            _freeze_timer = std::max(_freeze_timer, duration);
        }
        
        void add_poison(float damage, float interval){
            _poison_combo++;
            _poison_damage = damage;
            _poison_interval = (_poison_interval * (_poison_combo-1) + interval) / _poison_combo;  //求平均的poison interval
        }
        void set_poison_ready(bool v){ _poison_ready = v; }
        void decay_poison_combo() { _poison_combo -= 1; }
        
        void set_nearby_buff(bool v) { _has_nearby_buff = v; }
        void set_nearby_heal(bool v) { _has_nearby_heal = v; }
        
        
        
        // update - move
        void update(float dt) override {
            if(update_status(dt)) return; // 被冰就跳過
            if(get_position().x > _target_x){
                Vector2 temp = get_position();
                temp.x += get_speed().x * _slow_multiplier * dt;
                set_position(temp);
            }
            for(int i = 0; i < _behaviors.size(); i++){
                _behaviors[i]->apply(*this, dt);
            }
        }

};