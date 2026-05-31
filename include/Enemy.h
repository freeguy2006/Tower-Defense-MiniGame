// 敵人 (is-a character)，從右往左走到 target_x 停下
// 繼承可用: get_position(), set_position(), get_size(), is_active(), get_rect()
//          get_hp(), is_alive(), take_damage(float), get_speed(), set_speed()
#pragma once
#include "Character.h"
#include "BehaviorAbstract.h"
#include <algorithm>
#include <cmath>

enum enemy_type {SLIMEGREEN,SLIMEBLACK,SLIMERED,SLIMEPURPLE,SLIMEBLUE,FLYINGANGEL,FLYINGBIRD,FLYINGDRAGON,FLYINGWIND,GOBLIN};

class enemy: public character{
    private:
        enemy_type _enemy_type;
        float _target_x;
        int _reward;
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

    public:
        // constructor
        enemy(Vector2 position, Vector2 size, bool active, float hp, Vector2 speed, float target_x, enemy_type type, int reward)
        : character(position, size, active, hp, speed), _target_x(target_x), _enemy_type(type), _reward(reward){}
        // destructor
        virtual ~enemy(){
            for(int i = 0;i<_behaviors.size();i++){
                delete _behaviors[i];
            }
        }
        // update
        virtual void update(float dt){
            update_status(dt);
            if (get_position().x > get_target_x()) {
                Vector2 temp = get_position();
                temp.x += get_speed().x * get_slow_multiplier() * dt;
                set_position(temp);
            }
            for (auto b : get_behaviors()) {
                b->apply(*this, dt);
            }
        }
 
    protected:
        // update status (slow, freeze, poison)
        void update_status(float dt){
            // slow status
            if(_slow_timer > 0){
                _slow_timer -= dt;
            }else{
                _slow_multiplier = 1.0f;
            }
            // freeze（放在 slow 之後覆蓋 multiplier，讓敵人停住）
            if(_freeze_timer > 0){
                _freeze_timer -= dt;
                _slow_multiplier = 0.0f;
            }
            // poison status
            if(_poison_combo > 0){
                _poison_timer -= dt;
                if(_poison_timer < 0){
                    _poison_ready = true;
                    _poison_timer += _poison_interval;
                }
            }
        }

    public:
        //bahavior
        void add_behavior(enemy_behavior* behavior){ _behaviors.push_back(behavior); }
        const std::vector<enemy_behavior*>& get_behaviors() const { return _behaviors; }
        
        // type
        enemy_type get_enemy_type() const { return _enemy_type; }
        // reward
        int get_reward() const { return _reward; }
        // target_x
        float get_target_x() const { return _target_x; }
        
        //slow
        void apply_slow(float percent, float duration){
            _slow_multiplier = std::min(_slow_multiplier, 1.0f - percent);
            _slow_timer = std::max(_slow_timer, duration);
        }
        float get_slow_multiplier() const { return _slow_multiplier; }
        bool is_slowed() const { return _slow_timer > 0; }
        
        // freeze
        void apply_freeze(float duration){
            _freeze_timer = std::max(_freeze_timer, duration);
        }
        bool is_frozen() const { return _freeze_timer > 0; }
        
        //poison
        bool is_poison_ready(){ return _poison_ready; }        
        float get_poison_tick_damage() const { return _poison_damage * _poison_combo; }
        int get_poison_combo() const { return _poison_combo; }
        bool is_poisoned() const { return _poison_combo > 0; }
        void set_poison_ready(bool v){ _poison_ready = v; }
        void decay_poison_combo() { _poison_combo -= 1; }
        void add_poison(float damage, float interval){
            if(_poison_combo == 0) _poison_timer = interval;
            _poison_combo++;
            _poison_damage = damage;
            _poison_interval = (_poison_interval * (_poison_combo-1) + interval) / _poison_combo;  //求平均的poison interval
        }
        
        // buff
        bool has_nearby_buff() const { return _has_nearby_buff; }
        bool has_nearby_heal() const { return _has_nearby_heal; }
        void set_nearby_buff(bool v) { _has_nearby_buff = v; }
        void set_nearby_heal(bool v) { _has_nearby_heal = v; }
};