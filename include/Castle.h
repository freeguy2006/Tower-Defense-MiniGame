// 城堡 (is-a building)，固定在左邊，被打爆就 game over
// 繼承可用: get_position(), get_size(), set_position(), is_active(), get_rect()
//          get_hp(), is_alive(), take_damage(int)
#pragma once
#include "Building.h" 
class castle : public building{
    public:
        castle(Vector2 position, Vector2 size, bool active, int hp):building(position, size, active, hp){}
        ~castle() = default;
        void update(float dt) override {}

};