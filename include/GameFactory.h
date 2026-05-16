#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Castle.h"
#include "Projectile.h"

class game_factory {
    public:
        static constexpr float GROUND_Y = 640;
        static player create_player(Vector2 position) {
            return player(position, {64,64}, true, 50, {0,0}, 0.3, 1600.0, 800.0, GROUND_Y-64, 200.0);
        }
        static enemy create_enemy(Vector2 position) {
            return enemy(position, {64,64}, true, 5, {-200,0}, 0);
        }
        static castle create_castle(Vector2 position) {
            return castle(position, {248,496}, true, 100);
        }
        static projectile create_projectile(Vector2 position, Vector2 speed) {
            return projectile(position, {10,10}, true, 2, speed);
        }
};
