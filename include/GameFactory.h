#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Castle.h"
#include "FlyingEnemy.h"
#include "JumpBehavior.h"
#include "BuffBehavior.h"
#include "Projectile.h"

class game_factory {
    public:
        static constexpr float GROUND_Y = 765;

        // player(位置, 大小, 活著, 血量, 速度, 攻擊冷卻, 重力, 跳躍力, 地面Y, 移動速度)
        static player create_player(Vector2 position) {
            return player(position, {72,96}, true, 50, {0,0}, 0.3, 1600.0, 800.0, GROUND_Y-96, 200.0);
        }
        // castle(位置, 大小, 活著, 血量)
        static castle create_castle(Vector2 position) {
            return castle(position, {200,700}, true, 100);
        }
        // projectile(位置, 大小, 活著, 傷害, 速度)
        static projectile create_projectile(Vector2 position, Vector2 speed) {
            return projectile(position, {10,10}, true, 2, speed);
        }

        // enemy(位置, 碰撞箱大小, 活著, 血量, 速度, 目標X, 類型)
        // jump_behavior(地面Y, 小跳力道, 跳躍間隔)
        // jump_behavior(地面Y, 小跳力道, 大跳力道, 跳躍間隔)
        // buff_behavior(範圍, 減傷倍率, 增傷倍率, 加速倍率)
        // flying_enemy(位置, 碰撞箱大小, 活著, 血量, 速度, 目標X, 類型, 飄動幅度, 飄動速度)

        // 黑色（不跳）
        static enemy* create_enemy_black(Vector2 position) {
            return new enemy(position, {94,88}, true, 30, {-(float)GetRandomValue(90,110),0}, 0, SLIMEBLACK);
        }
        // 綠色（小跳）
        static enemy* create_enemy_green(Vector2 position) {
            enemy* e = new enemy(position, {60,54}, true, 5, {-(float)GetRandomValue(180,220),0}, 0, SLIMEGREEN);
            e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue(500,650), (float)GetRandomValue(145,155)/100.0f));
            return e;
        }
        // 紅色（小跳）
        static enemy* create_enemy_red(Vector2 position) {
            enemy* e = new enemy(position, {67,60}, true, 3, {-(float)GetRandomValue(380,420),0}, 0, SLIMERED);
            e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue(700,900), (float)GetRandomValue(95,145)/100.0f));
            return e;
        }
        // 紫色（小跳+大跳交替）
        static enemy* create_enemy_purple(Vector2 position) {
            enemy* e = new enemy(position, {63,61}, true, 10, {-(float)GetRandomValue(170,190),0}, 0, SLIMEPURPLE);
            e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue(750,850),(float)GetRandomValue(1200,1400),(float)GetRandomValue(135,155)/100.0f));
            return e;
        }
        // 藍色（buff）
        static enemy* create_enemy_blue(Vector2 position) {
            enemy* e = new enemy(position, {61,54}, true, 10, {-(float)GetRandomValue(170,190),0}, 0, SLIMEBLUE);
            e->add_behavior(new buff_behavior(385, 0.5f, 3.0f, 1.1f));
            return e;
        }
        // 天使
        static flying_enemy* create_enemy_angel(Vector2 position) {
            return new flying_enemy(position, {88,81}, true, 10, {-(float)GetRandomValue(170,200),0}, 0, FLYINGANGEL, (float)GetRandomValue(100,160),(float)GetRandomValue(200,250)/100.0f);
        }
        // 小鳥
        static flying_enemy* create_enemy_bird(Vector2 position) {
            return new flying_enemy(position, {39,37}, true, 5, {-(float)GetRandomValue(170,200),0}, 0, FLYINGBIRD, (float)GetRandomValue(100,160),(float)GetRandomValue(300,500)/100.0f);
        }
        // 飛龍（Boss）
        static flying_enemy* create_enemy_dragon(Vector2 position) {
            return new flying_enemy(position, {136,136}, true, 50, {-(float)GetRandomValue(150,180),0}, 0, FLYINGDRAGON, (float)GetRandomValue(100,120),(float)GetRandomValue(100,150)/100.0f);
        }

};
