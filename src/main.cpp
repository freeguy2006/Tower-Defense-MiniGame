#include "Game.h"
int main() {
    InitWindow(2400, 900, "Tower Defense Game");
    SetExitKey(0);  // 取消 ESC 關閉視窗
    SetTargetFPS(60);
    player p1 = game_factory::create_player({640, 360});
    castle c1 = game_factory::create_castle({50, 700});
    enemy e1 = game_factory::create_enemy({2300, 800});
    game g(p1, c1);
    g.add_enemy(e1);
    g.run();
    CloseWindow();
    return 0;
}
