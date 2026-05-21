#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Castle.h"
#include "FlyingEnemy.h"
#include "JumpBehavior.h"
#include "BuffBehavior.h"
#include "HealBehavior.h"
#include "Projectile.h"
#include "Goblin.h"
enum weapon_type { MUD, ARROW, STONE, ICE_SLOW, POISON, PIERCING_ARROW, IRON_BALL, FIRE_BALL, ROCKET, ICE_FREEZE, MISSILE, WEAPON_COUNT};
enum potion_type { HEAL_PLAYER_POTION, HEAL_CASTLE_POTION, ATTACK_POTION, ATTACK_SPEED_POTION, SHIELD_POTION, MOVE_SPEED_POTION, REGENERATION_POTION, POTION_COUNT};
static constexpr const char* WEAPON_NAME[] = {"Mud", "Arrow", "Stone Arrow", "Ice Slow", "Poison Arrow", "Piercing Arrow", "Iron Ball", "Fire Ball", "Rocket", "Ice Freeze", "Missile"};
static constexpr const char* POTION_NAME[] = {"Heal Player", "Heal Castle", "Attack", "Attack Speed", "Shield", "Move Speed", "Regenerate"};
static constexpr int WEAPON_COST[] = {0, 30, 30, 50, 50, 50, 50, 50, 100, 100, 100};  // 11 個
static constexpr int POTION_COST[] = {15, 15, 30, 30, 30, 30, 30};  // 7 個

class game_factory {
    public:
        static constexpr float GROUND_Y = 765;
        // player(位置, 大小, 活著, 血量, 速度, 攻擊冷卻, 重力, 跳躍力, 地面Y, 移動速度)
        static player create_player(Vector2 position) {
            return player(position, {72,96}, true, 50.0f, {0,0}, 0.3f, 1600.0f, 800.0f, GROUND_Y-96, 200.0f);
        }
        // castle(位置, 大小, 活著, 血量)
        static castle create_castle(Vector2 position) {
            return castle(position, {200,700}, true, 100.0f);
        }
        // projectile(位置, 大小, 活著, 傷害, 速度)
        static projectile create_projectile(Vector2 position, float damage, Vector2 speed) {
            return projectile(position, {10,10}, true, damage, speed);
        }
        // 商店哥布林
        static goblin* create_goblin(Vector2 position){
            return new goblin(position, {62, 80}, true, 30, {-150.0f, 0}, GetRandomValue(0, 20));
        }


        // enemy(位置, 碰撞箱大小, 活著, 血量, 速度, 目標X, 類型)
        // jump_behavior(地面Y, 小跳力道, 跳躍間隔)
        // jump_behavior(地面Y, 小跳力道, 大跳力道, 跳躍間隔)
        // buff_behavior(範圍, 減傷倍率, 增傷倍率, 加速倍率)
        // flying_enemy(位置, 碰撞箱大小, 活著, 血量, 速度, 目標X, 類型, 飄動幅度, 飄動速度)

        // 黑色（不跳）
        static enemy* create_enemy_black(Vector2 position, float hp_multiplier) {
            return new enemy(position, {94,88}, true, (int)(50.0f*hp_multiplier), {-(float)GetRandomValue(50,80),0}, 0, SLIMEBLACK, GetRandomValue(2,4));
        }
        // 綠色（小跳）
        static enemy* create_enemy_green(Vector2 position, float hp_multiplier) {
            enemy* e = new enemy(position, {60,54}, true, (int)(8.0f*hp_multiplier), {-(float)GetRandomValue(150,200),0}, 0, SLIMEGREEN, GetRandomValue(0,2));
            e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue(500,650), (float)GetRandomValue(145,155)/100.0f));
            return e;
        }
        // 紅色（小跳）
        static enemy* create_enemy_red(Vector2 position, float hp_multiplier) {
            enemy* e = new enemy(position, {67,60}, true, (int)(3.0f*hp_multiplier), {-(float)GetRandomValue(350,500),0}, 0, SLIMERED, GetRandomValue(1,2));
            e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue(700,900), (float)GetRandomValue(95,145)/100.0f));
            return e;
        }
        // 紫色（小跳+大跳交替）
        static enemy* create_enemy_purple(Vector2 position, float hp_multiplier) {
            enemy* e = new enemy(position, {63,61}, true, (int)(20.0f*hp_multiplier), {-(float)GetRandomValue(130,190),0}, 0, SLIMEPURPLE, GetRandomValue(2,5));
            e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue(750,850),(float)GetRandomValue(1200,1400),(float)GetRandomValue(135,155)/100.0f));
            return e;
        }
        // 藍色（buff）
        static enemy* create_enemy_blue(Vector2 position, float hp_multiplier) {
            enemy* e = new enemy(position, {61,54}, true, (int)(12.0f*hp_multiplier), {-(float)GetRandomValue(110,130),0}, 0, SLIMEBLUE, GetRandomValue(4,7));
            e->add_behavior(new buff_behavior(385, 0.5f, 3.0f, 1.1f));
            return e;
        }
        // 天使
        static flying_enemy* create_enemy_angel(Vector2 position, float hp_multiplier) {
            flying_enemy* e = new flying_enemy(position, {88,81}, true, (int)(15.0f*hp_multiplier), {-(float)GetRandomValue(120,160),0}, 0, FLYINGANGEL, (float)GetRandomValue(100,160),(float)GetRandomValue(200,250)/100.0f,GetRandomValue(4,7));
            e->add_behavior(new heal_behavior(2,2,200));
            return e;
        }
        // 小鳥
        static flying_enemy* create_enemy_bird(Vector2 position, float hp_multiplier) {
            return new flying_enemy(position, {39,37}, true, (int)(5.0f*hp_multiplier), {-(float)GetRandomValue(170,200),0}, 0, FLYINGBIRD, (float)GetRandomValue(100,160),(float)GetRandomValue(300,500)/100.0f,GetRandomValue(1,2));
        }
        // 飛龍（Boss）
        static flying_enemy* create_enemy_dragon(Vector2 position, float hp_multiplier) {
            return new flying_enemy(position, {136,136}, true, (int)(100.0f*hp_multiplier), {-(float)GetRandomValue(50,80),0}, 0, FLYINGDRAGON, (float)GetRandomValue(100,120),(float)GetRandomValue(100,150)/100.0f,GetRandomValue(25,50));
        } 



        
        // 武器效果
        static void apply_weapon(projectile& p, weapon_type type){
            p.set_weapon_type(type);
            switch(type){
                case MUD:
                    break;
                case ARROW:
                    p.set_damage(p.get_damage() * 1.1f);
                    break;
                case STONE:
                    p.set_damage(p.get_damage() * 1.25f);
                    break;
                case ICE_SLOW:
                    p.set_damage(p.get_damage() * 1.4f);
                    p.set_slow_percent(0.5f);
                    p.set_slow_duration(2.0f);
                    break;
                case POISON:
                    p.set_damage(p.get_damage() * 1.2f);
                    p.set_poison_damage(p.get_damage() * 0.1f);
                    p.set_poison_interval(1.0f);
                    break;
                case PIERCING_ARROW:
                    p.set_damage(p.get_damage() * 1.1f);
                    p.set_piercing(true);
                    break;
                case IRON_BALL:
                    p.set_damage(p.get_damage() * 1.05f);
                    p.set_crit_chance(0.03f);
                    p.set_crit_hp_percent(0.9f);
                    break;
                case FIRE_BALL:
                    p.set_damage(p.get_damage() * 1.1f);
                    p.set_splash_damage(p.get_damage() * 0.5f);
                    p.set_splash_range(140.0f);
                    break;
                case ROCKET:
                    p.set_damage(p.get_damage() * 1.5f);
                    break;
                case ICE_FREEZE:
                    p.set_damage(p.get_damage() * 1.2f);
                    p.set_freeze_duration(1.0f);
                    break;
                case MISSILE:
                    p.set_damage(p.get_damage() * 1.1f);
                    p.set_splash_range(200.0f);
                    p.set_splash_damage(p.get_damage() * 0.5f);
                    p.set_cooldown_multiplier(5.0f);
                    break;
            }
        }
};