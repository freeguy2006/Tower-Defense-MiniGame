// base class

#pragma once

class enemy;

class enemy_behavior{
    public:
        virtual ~enemy_behavior()= default;
        virtual void apply(enemy &e, float dt) = 0;    
};