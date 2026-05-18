#pragma once
#include "GameObject.h"

class coin: public game_object{
    private:
        Vector2 _speed;
        float _gravity = 1600;
        float _ground_y;
        float _lifetime = 15;  
        float _timer = 0;
        int _value;
    public:
        coin(Vector2 position, int value, float ground_y) 
        : game_object(position, {41,39}, true), _value(value), _ground_y(ground_y), _speed({-(float)GetRandomValue(-15,15),-(float)GetRandomValue(300,500)}){}
        
        void update(float dt)override{
            Vector2 pos = get_position();
            _speed.y += _gravity * dt;
            pos.x += _speed.x * dt;
            pos.y += _speed.y * dt;
            if(pos.y>=_ground_y){
                pos.y = _ground_y;
                _speed.y = 0;
                _speed.x = 0;
            }
            set_position(pos);
            _timer += dt;
        }
        bool is_expired(){ return _timer>=_lifetime; }
        int get_value(){ return _value; }
};