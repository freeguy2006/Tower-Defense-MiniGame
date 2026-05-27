#pragma once
#include <cmath>
#include <vector>
#include <algorithm>
#include "Enemy.h"
#include "BehaviorAbstract.h"

// ==========================================
// 1. Jump Behavior (跳躍行為)
// ==========================================
class jump_behavior : public enemy_behavior {
    private:
        float _base_y;
        float _vertical_speed = 0;
        float _small_jump_force = 0;
        float _big_jump_force = 0;
        float _gravity = 1600;
        float _jump_timer = 0;
        float _jump_cooldown;
        int _jump_count = 0;
    public:
        jump_behavior(float base_y, float jump_force, float cooldown)
            : _base_y(base_y), _small_jump_force(jump_force), _jump_cooldown(cooldown) {}
        jump_behavior(float base_y, float small_jump_force, float big_jump_force, float cooldown)
            : _base_y(base_y), _small_jump_force(small_jump_force), _big_jump_force(big_jump_force), _jump_cooldown(cooldown) {}
        
        void apply(enemy &e, float dt) override {
            if (e.is_frozen()) return;
            float slow = e.get_slow_multiplier();
            Vector2 enemy_pos = e.get_position();
            _jump_timer += dt * slow;
            if (_jump_timer >= _jump_cooldown && enemy_pos.y >= _base_y) {
                _jump_timer = 0;
                if (_jump_count % 2 == 0 || _big_jump_force == 0) {
                    _vertical_speed = -_small_jump_force;
                } else {
                    _vertical_speed = -_big_jump_force;
                }
                _jump_count++;
            }
            _vertical_speed += _gravity * dt * slow;
            enemy_pos.y += _vertical_speed * dt * slow;
            if (enemy_pos.y >= _base_y) {
                enemy_pos.y = _base_y;
                _vertical_speed = 0;
            }
            e.set_position(enemy_pos);
        }
};

// ==========================================
// 2. Buff Behavior (光環 Buff 行為)
// ==========================================
class buff_behavior : public enemy_behavior {
    private:
        float _buff_range;
        float _damage_reduction;
        float _damage_boost;
        float _speed_boost;
    public:
        buff_behavior(float range, float reduction, float dmg_boost, float spd_boost)
            : _buff_range(range), _damage_reduction(reduction), _damage_boost(dmg_boost), _speed_boost(spd_boost) {}
        void apply(enemy& e, float dt) override {}
        float get_buff_range() const { return _buff_range; }
        float get_damage_reduction() const { return _damage_reduction; }
        float get_damage_boost() const { return _damage_boost; }
        float get_speed_boost() const { return _speed_boost; }
};

// ==========================================
// 3. Heal Behavior (範圍治療行為)
// ==========================================
class heal_behavior : public enemy_behavior {
    private:
        float _heal_amount;
        float _heal_cooldown;
        float _heal_timer = 0;
        float _heal_range;
        bool _can_heal = false;
    public:
        heal_behavior(float amount, float cooldown, float range)
            : _heal_amount(amount), _heal_cooldown(cooldown), _heal_range(range) {}
        void apply(enemy& e, float dt) override {
            _heal_timer += dt;
            if (_heal_timer >= _heal_cooldown) {
                _can_heal = true;
                _heal_timer = 0;
            } else {
                _can_heal = false;
            }
        }
        bool is_able_to_heal() const { return _can_heal; }
        float get_heal_range() const { return _heal_range; }
        float get_heal_amount() const { return _heal_amount; }
        float get_heal_cooldown() const { return _heal_cooldown; }
};

// ==========================================
// 4. Fall and Float Behavior (旋風怪下墜浮動行為)
// ==========================================
class fall_and_float_behavior : public enemy_behavior{
    private:
        float _target_y;
        float _fall_speed = 600.0f;
        float _fly_timer = 0.0f;
        float _fly_amplitude;
        float _fly_speed;
        bool _has_landed = false;
    public: 
        fall_and_float_behavior(float target_y, float fly_amplitude, float fly_speed)
        : _target_y(target_y), _fly_amplitude(fly_amplitude), _fly_speed(fly_speed) {}
        void apply(enemy& e, float dt) override {
            if(e.is_frozen()) return;
            Vector2 enemy_pos = e.get_position();
            float slow = e.get_slow_multiplier();
            if(_has_landed == false){
                enemy_pos.y += _fall_speed * dt * slow;
                if(enemy_pos.y>=_target_y){
                    enemy_pos.y = _target_y;
                    _has_landed = true;
                }
            }else{
                _fly_timer += dt * slow;
                enemy_pos.y = _target_y + sin(_fly_timer * _fly_speed) * _fly_amplitude;
            }
            e.set_position(enemy_pos);
        }
};
