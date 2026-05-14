#include "Player.h"

void player::update(float dt){
    _attack_timer += dt;
    if(IsKeyDown(KEY_A)){
        set_speed({-_move_speed, get_speed().y});
    }else if(IsKeyDown(KEY_D)){
        set_speed({_move_speed, get_speed().y});
    }else{
        set_speed({0, get_speed().y});
    }

    if(IsKeyPressed(KEY_W) && is_on_ground()){
        set_speed({get_speed().x, -_jump_force});
    }
   
    Vector2 temp = get_position();

    set_speed({get_speed().x, get_speed().y + _gravity * dt});
    temp.x += get_speed().x * dt;
    temp.y += get_speed().y * dt;
    if(temp.y >= _ground_y){
        temp.y = _ground_y;
        set_speed({get_speed().x, 0});
    }
    set_position(temp);
    

}
/*
1. ad 左右移動
2. w 跳
*/ 