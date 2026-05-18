#pragma once
#include "Enemy.h"
#include <cmath>
class flying_enemy : public enemy{
    private:
        float _fly_timer; // 頻率
        float _fly_amplitude;  // 幅度
        float _fly_speed; 
        float _base_y; // y 基準
    public:
        flying_enemy(Vector2 position, Vector2 size, bool active, int hp, Vector2 speed, int target_x, enemy_type type, float amplitude, float fly_speed)
        : enemy(position, size, active, hp, speed, target_x, type), _fly_timer(0), _fly_amplitude(amplitude), _fly_speed(fly_speed), _base_y(position.y){}
        
        
        void update(float dt)override{
            if(get_position().x > 0){
                Vector2 temp = get_position();
                temp.x += get_speed().x * dt;
                _fly_timer += dt;
                temp.y = _base_y + sin(_fly_timer*_fly_speed)*_fly_amplitude;
                set_position(temp);    
            }
            for (int i = 0;i<get_behaviors().size();i++){
                get_behaviors()[i]->apply(*this, dt);
            }
        }

};