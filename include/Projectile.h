// 子彈 (is-a game_object)，自動沿 speed 方向移動
// 繼承可用: get_position(), set_position(), get_size(), is_active(), get_rect()
// 自己的: get_damage(), get_speed()
#pragma once
#include "GameObject.h"

class projectile : public game_object{
    private:
        int _damage;                        // 基礎傷害
        Vector2 _speed;                     // 移動速度
        int _weapon_type = 0;               // 武器類型
        bool _piercing = false;             // 穿透
        float _slow_percent = 0;            // 緩速%數
        float _slow_duration = 0;           // 緩速時間
        float _freeze_duration = 0;         // 冰凍時間
        float _splash_range = 0;            // 範圍傷害範圍
        int _splash_damage = 0;             // 範圍傷害
        int _poison_damage = 0;             // 毒傷害
        float _poison_interval = 3.0f;      // 毒間隔
        float _crit_chance = 0;             // 暴擊機率
        int _crit_damage = 0;               // 暴擊傷害
        float _crit_multiplier = 1.0f;         // 暴擊倍率
        float _crit_hp_percent = 0;         // HP百分比暴擊
        float _cooldown_multiplier = 1.0f;  // 攻速倍率

    public:
        projectile(Vector2 position, Vector2 size, bool active, int damage, Vector2 speed): game_object(position, size, active), _damage(damage), _speed(speed){}
        ~projectile() = default;
        void update(float dt)override{
            Vector2 temp = get_position();
            temp = {temp.x+_speed.x*dt,temp.y+_speed.y*dt};
            set_position(temp);
        }
        // setter
        void set_damage(int damage) { _damage = damage; }
        void set_speed(Vector2 speed) { _speed = speed; }
        void set_weapon_type(int weapon_type) { _weapon_type = weapon_type; }
        void set_piercing(bool piercing) { _piercing = piercing; }
        void set_slow_percent(float slow_percent) { _slow_percent = slow_percent; }
        void set_slow_duration(float slow_duration) { _slow_duration = slow_duration; }
        void set_freeze_duration(float freeze_duration) { _freeze_duration = freeze_duration; }
        void set_splash_range(int splash_range) { _splash_range = splash_range; }
        void set_splash_damage(int splash_damage) { _splash_damage = splash_damage; }
        void set_poison_damage(int poison_damage) { _poison_damage = poison_damage; }
        void set_poison_interval(float poison_interval) { _poison_interval = poison_interval; }
        void set_crit_chance(float crit_chance) { _crit_chance = crit_chance; }
        void set_crit_damage(int crit_damage) { _crit_damage = crit_damage; }
        void set_crit_multiplier(float crit_multiplier) { _crit_multiplier = crit_multiplier; }
        void set_crit_hp_percent(float crit_hp_percent) { _crit_hp_percent = crit_hp_percent; }
        void set_cooldown_multiplier(float cooldown_multiplier) { _cooldown_multiplier = cooldown_multiplier; }
        // getter
        int get_damage() const { return _damage; }
        Vector2 get_speed() const { return _speed; }
        bool is_piercing() const { return _piercing; }
        float get_slow_percent() const { return _slow_percent; }
        float get_slow_duration() const { return _slow_duration; }
        float get_freeze_duration() const { return _freeze_duration; }
        float get_splash_range() const { return _splash_range; }
        int get_splash_damage() const { return _splash_damage; }
        int get_poison_damage() const { return _poison_damage; }
        float get_poison_interval() const { return _poison_interval; }
        float get_crit_chance() const { return _crit_chance; }
        int  get_crit_damage() const { return _crit_damage; }
        float get_crit_multiplier() const { return _crit_multiplier; }
        float get_crit_hp_percent() const { return _crit_hp_percent; }
        float get_cooldown_multiplier() const { return _cooldown_multiplier; }
        int get_weapon_type() const { return _weapon_type; }
};