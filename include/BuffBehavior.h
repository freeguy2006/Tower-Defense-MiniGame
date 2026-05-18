#pragma once
#include "EnemyBehavior.h"
#include "Enemy.h"
#include <cmath>
class buff_behavior : public enemy_behavior{
    private:
        float _buff_range;
        float _damage_reduction;
        float _damage_boost;
        float _speed_boost;
    public:
        buff_behavior(float range, float reduction, float dmg_boost, float spd_boost):_buff_range(range),_damage_reduction(reduction),_damage_boost(dmg_boost), _speed_boost(spd_boost){}
        void apply(enemy& e, float dt) override {}
        float get_buff_range() const { return _buff_range; }
        float get_damage_reduction() const { return _damage_reduction; }
        float get_damage_boost() const { return _damage_boost; }
        float get_speed_boost() const { return _speed_boost; }

};