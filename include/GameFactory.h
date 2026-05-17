#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Castle.h"
#include "Projectile.h"

class game_factory {
    public:
        static constexpr float GROUND_Y = 640;

        // player(位置, 大小, 活著, 血量, 速度, 攻擊冷卻, 重力, 跳躍力, 地面Y, 移動速度)
        static player create_player(Vector2 position) {
            return player(position, {64,64}, true, 50, {0,0}, 0.3, 1600.0, 800.0, GROUND_Y-64, 200.0);
        }
        // enemy(位置, 大小, 活著, 血量, 速度, 目標X, type)
        static enemy create_enemy_green(Vector2 position) {
            return enemy(position, {64,64}, true, 5, {-200,0}, 0, GREEN);
        }
        static enemy create_enemy_black(Vector2 position) {
            return enemy(position, {96,96}, true, 30, {-100,0}, 0, BLACK);
        }
        static enemy create_enemy_red(Vector2 position) {
            return enemy(position, {32,32}, true, 3, {-400,0}, 0, RED);
        }
        static enemy create_enemy_purple(Vector2 position) {
            return enemy(position, {72,72}, true, 10, {-180,0}, 0, PURPLE);
        }
        static enemy create_enemy_blue(Vector2 position) {
            return enemy(position, {72,72}, true, 10, {-180,0}, 0, BLUE);
        }
        // castle(位置, 大小, 活著, 血量)
        static castle create_castle(Vector2 position) {
            return castle(position, {248,496}, true, 100);
        }
        // projectile(位置, 大小, 活著, 傷害, 速度)
        static projectile create_projectile(Vector2 position, Vector2 speed) {
            return projectile(position, {10,10}, true, 2, speed);
        }
};
