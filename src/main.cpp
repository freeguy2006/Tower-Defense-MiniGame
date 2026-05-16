#include "Game.h"
int main() {
    InitWindow(2400, 900, "Tower Defense Game");
    SetExitKey(0);  // 取消 ESC 關閉視窗
    SetTargetFPS(60);
    player p1 = game_factory::create_player({640, game_factory::GROUND_Y-30});
    castle c1 = game_factory::create_castle({100, game_factory::GROUND_Y-400});
    enemy e1 = game_factory::create_enemy({2300, game_factory::GROUND_Y-64});
    {
        game g(p1, c1);
        g.run();
    }
    CloseWindow(); 
    return 0;
}
