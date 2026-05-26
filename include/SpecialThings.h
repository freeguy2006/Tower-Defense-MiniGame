#pragma once
#include "GameObject.h"

class crown: public game_object{
    private:
        Vector2 _speed;
        float _gravity = 800;
        float _ground_y;
    public:
        crown(Vector2 position, float ground_y) 
        : game_object(position, {88,68}, true), _ground_y(ground_y),_speed({0,0}){}
        
        void update(float dt)override{
            Vector2 pos = get_position();
            _speed.y += _gravity * dt;
            pos.y += _speed.y * dt;
            if(pos.y>=_ground_y){
                pos.y = _ground_y;
                _speed.y = 0;
            }
            set_position(pos);
        }
};

class special_gift: public game_object{
    private:
        Vector2 _speed;
        float _gravity = 800;
        float _ground_y;
    public:
        special_gift(Vector2 position, float ground_y) 
        : game_object(position, {106,106}, true), _ground_y(ground_y),_speed({0,0}){}
        
        void update(float dt)override{
            Vector2 pos = get_position();
            _speed.y += _gravity * dt;
            pos.y += _speed.y * dt;
            if(pos.y>=_ground_y){
                pos.y = _ground_y;
                _speed.y = 0;
            }
            set_position(pos);
        }
};