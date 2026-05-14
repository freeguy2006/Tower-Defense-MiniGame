// 子彈 (is-a game_object)，自動沿 speed 方向移動
// 繼承可用: get_position(), set_position(), get_size(), is_active(), get_rect()
// 自己的: get_damage(), get_speed()
#pragma once
#include "GameObject.h"

class projectile : public game_object{
    private:
        int _damage;
        Vector2 _speed;
    public:
        projectile(Vector2 position, Vector2 size, bool active, int damage, Vector2 speed): game_object(position, size, active), _damage(damage), _speed(speed){}
        ~projectile() = default;
        void update(float dt)override{
            Vector2 temp = get_position();
            temp = {temp.x+_speed.x*dt,temp.y+_speed.y*dt};
            set_position(temp);
        }
        int get_damage() const { return _damage; }
        Vector2 get_speed() const { return _speed; }
};