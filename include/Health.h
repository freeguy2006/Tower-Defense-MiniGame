
#pragma once
class health{
    private:
        float _hp;
        float _max_hp;
    public:
        health(float hp): _hp(hp), _max_hp(hp){}
        ~health() = default;

        void heal(float amount){
            _hp += amount;
            if(_hp > _max_hp) _hp = _max_hp;
        }

        float get_hp() const { return _hp; }
        float get_max_hp() const { return _max_hp; }
        bool is_alive() const { return _hp > 0; }
        void take_damage(float damage){
            _hp -= damage;
            if(_hp < 0) _hp = 0;
        }
        void increase_max_hp(float amount){
            _max_hp += amount;
            _hp += amount;
        }
};
