// 抽象基底，所有遊戲物件的根類別
// 可用: get_position(), get_size(), set_position(Vector2), is_active(), get_rect(), update(float dt)
#pragma once // 引入一次
#include "raylib.h"
class game_object {
    private:
        Vector2 _position;
        Vector2 _size;
        bool _active;
    public:
        game_object(Vector2 position, Vector2 size, bool active){
            _position = position;
            _size = size;
            _active = active;
        };
        virtual ~game_object() = default;

        // getter 
        Vector2 get_position() const { return _position; }
        Vector2 get_size() const { return _size; }
        //setter
        void set_position(Vector2 position){
            _position = position;
        }
        
        bool is_active() const { return _active; }
        virtual void update(float dt) = 0; // dt : 時間差

        //for test 
        Rectangle get_rect() const {
            return {_position.x, _position.y, _size.x, _size.y};
        }

};


