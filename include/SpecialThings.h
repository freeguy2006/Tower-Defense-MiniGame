#pragma once
#include "FallingObject.h"

class crown: public falling_object{
    public:
        crown(Vector2 position, float ground_y) 
        : falling_object(position, {88,68}, true, ground_y, 800.0f, {0, 0}) {}
        
        void update(float dt)override{
            update_physics(dt);
        }
};

class special_gift: public falling_object{
    public:
        special_gift(Vector2 position, float ground_y) 
        : falling_object(position, {106,106}, true, ground_y, 800.0f, {0, 0}){}
        
        void update(float dt)override{
            update_physics(dt);
        }
};