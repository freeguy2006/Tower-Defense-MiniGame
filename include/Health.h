// 獨立元件，用 has-a 組合進其他 class
// 可用: get_hp(), is_alive(), take_damage(int)
#pragma once
class health{
    private:
        int _hp;
        int _max_hp;
    public:
        health(int hp): _hp(hp), _max_hp(hp){}
        ~health() = default;
        
        void heal(float amount){
            _hp += amount;
            if(_hp > _max_hp) _hp = _max_hp;
        }

        int get_hp() const { return _hp; }
        int get_max_hp() const { return _max_hp; }
        bool is_alive() const { return _hp > 0; }
        void take_damage(int damage){
            _hp -= damage;
            if(_hp < 0) _hp = 0;
        }
};
