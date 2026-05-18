// 敵人 (is-a character)，從右往左走到 target_x 停下
// 繼承可用: get_position(), set_position(), get_size(), is_active(), get_rect()
//          get_hp(), is_alive(), take_damage(int), get_speed(), set_speed()
#pragma once
#include "Character.h"
#include "EnemyBehavior.h"
enum enemy_type {SLIMEGREEN,SLIMEBLACK,SLIMERED,SLIMEPURPLE,SLIMEBLUE,FLYINGANGEL,FLYINGBIRD,FLYINGDRAGON};
class enemy : public character{
    private:
        enemy_type _enemy_type;
        int _target_x;
        int _reward;
        Vector2 _base_speed;
        std::vector<enemy_behavior*> _behaviors;
    public:
        enemy(Vector2 position, Vector2 size, bool active, int hp, Vector2 speed, int target_x, enemy_type type, int reward) 
        : character(position, size, active, hp, speed), _target_x(target_x), _enemy_type(type), _base_speed(speed), _reward(reward){}
        
        virtual ~enemy(){
            for(int i = 0;i<_behaviors.size();i++){
                delete _behaviors[i];
            }
        };

        void add_behavior(enemy_behavior* behavior){ _behaviors.push_back(behavior); }
        std::vector<enemy_behavior*> get_behaviors() const { return _behaviors; }
        void reset_speed(){ set_speed(_base_speed); }
        Vector2 get_base_speed() const { return _base_speed; }
        enemy_type get_enemy_type() const { return _enemy_type; }
        int get_reward() const { return _reward; }
        void update(float dt) override {
            if (get_position().x > _target_x) {
                Vector2 temp = get_position();
                temp.x += get_speed().x * dt;
                set_position(temp);
            }
            for (int i = 0;i<_behaviors.size();i++){
                _behaviors[i]->apply(*this, dt);
            }
        }
        
};