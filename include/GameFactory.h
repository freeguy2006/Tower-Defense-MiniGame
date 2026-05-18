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
        static constexpr float GROUND_Y = 640;

        // player(位置, 大小, 活著, 血量, 速度, 攻擊冷卻, 重力, 跳躍力, 地面Y, 移動速度)
        static player create_player(Vector2 position) {
            return player(position, {64,64}, true, 50, {0,0}, 0.3, 1600.0, 800.0, GROUND_Y-64, 200.0);
        }
        // castle(位置, 大小, 活著, 血量)
        static castle create_castle(Vector2 position) {
            return castle(position, {248,496}, true, 100);
        }
        // projectile(位置, 大小, 活著, 傷害, 速度)
        static projectile create_projectile(Vector2 position, Vector2 speed) {
            return projectile(position, {10,10}, true, 2, speed);
        }
        // 黑色（不跳）
        static enemy* create_enemy_black(Vector2 position) {
            return new enemy(position, {96,96}, true, 30, {-100,0}, 0, SLIMEBLACK);
        }
        // 綠色（小跳）
        static enemy* create_enemy_green(Vector2 position) {
            enemy* e = new enemy(position, {64,64}, true, 5, {-200,0}, 0, SLIMEGREEN);
            e->add_behavior(new jump_behavior(position.y, 800, 1.5f));
            return e;
        }
        // 紅色（小跳）
        static enemy* create_enemy_red(Vector2 position) {
            enemy* e = new enemy(position, {32,32}, true, 3, {-400,0}, 0, SLIMERED);
            e->add_behavior(new jump_behavior(position.y, 800, 1.0f));
            return e;
        }
        // 紫色（小跳+大跳交替）
        static enemy* create_enemy_purple(Vector2 position) {
            enemy* e = new enemy(position, {72,72}, true, 10, {-180,0}, 0, SLIMEPURPLE);
            e->add_behavior(new jump_behavior(position.y, 800, 1400, 1.5f));
            return e;
        }
        // 藍色（buff）
        static enemy* create_enemy_blue(Vector2 position) {
            enemy* e = new enemy(position, {72,72}, true, 10, {-180,0}, 0, SLIMEBLUE);
            e->add_behavior(new buff_behavior(385, 0.5f, 3.0f, 1.0f));
            return e;
        }
        // 飛行怪也改成回傳指標
        static flying_enemy* create_enemy_angel(Vector2 position) {
            return new flying_enemy(position, {64,64}, true, 10, {-180,0}, 0, FLYINGANGEL, 80, 2.5);
        }
        // bird, dragon 同理
        static flying_enemy* create_enemy_bird(Vector2 position) {
            return new flying_enemy(position, {32,32}, true, 5, {-200,0}, 0, FLYINGBIRD, 120, 3.0);
        }
        static flying_enemy* create_enemy_dragon(Vector2 position) {
            return new flying_enemy(position, {128,128}, true, 50, {-150,0}, 0, FLYINGDRAGON, 150, 2.0);
        }

};
