#pragma once
#include "FallingObject.h"

class coin: public falling_object{
    private:
        float _lifetime = 20;  
        float _timer = 0;
        int _value;
    public:
        coin(Vector2 position, int value, float ground_y) 
        : falling_object(position, {41, 39}, true, ground_y, 1600.0f, {-(float)GetRandomValue(-15, 15), -(float)GetRandomValue(300, 500)}), _value(value) {}
        
        void update(float dt)override{
            update_physics(dt); 
            _timer += dt;
        }
        bool is_expired() const { return _timer>=_lifetime; }
        int get_value() const { return _value; }
};