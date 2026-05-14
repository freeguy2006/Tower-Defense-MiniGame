#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Castle.h"
#include "Projectile.h"

class game_factory {
    public:
        static player create_player(Vector2 position) {
            return player(position, {30,30}, true, 100, {0,0}, 0.3, 1600.0, 800.0, 800.0, 200.0);
        }
        static enemy create_enemy(Vector2 position) {
            return enemy(position, {30,30}, true, 50, {-100,0}, 500);
        }
        static castle create_castle(Vector2 position) {
            return castle(position, {100,150}, true, 500);
        }
        static projectile create_projectile(Vector2 position, Vector2 speed) {
            return projectile(position, {10,10}, true, 100, speed);
        }
};
