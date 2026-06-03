#pragma once

class enemy;

// 行為抽象基底
class enemy_behavior {
public:
    virtual ~enemy_behavior() = default;
    virtual void apply(enemy& e, float dt) = 0;
    virtual bool has_buff() const { return false; }
    virtual float get_buff_range() const { return 0.0f; }
    virtual float get_damage_reduction() const { return 1.0f; }
    virtual float get_damage_boost() const { return 1.0f; }
    virtual float get_speed_boost() const { return 1.0f; }
    virtual bool has_heal() const { return false; }
    virtual bool is_able_to_heal() const { return false; }
    virtual float get_heal_range() const { return 0.0f; }
    virtual float get_heal_amount() const { return 0.0f; }
    virtual float get_heal_cooldown() const { return 0.0f; }
};