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
struct enemy_value{
    float hp;
    Vector2 speed;          // {min_speed, max_speed}
    Vector2 reward;         // {min_reward, max_reward}
    Vector2 jump_small;     // {min, max} 小跳力道，{0,0} = 不跳
    Vector2 jump_big;       // {min, max} 大跳力道，{0,0} = 沒有大跳
    Vector2 jump_cooldown;  // {min, max} 跳躍間隔 (÷100)
    Vector2 fly_amplitude;  // {min, max} 飛行振幅，{0,0} = 不飛
    Vector2 fly_speed;      // {min, max} 飛行頻率 (÷100)
};
struct upgrade_value{
    const char* name;
    int cost_base;
    int cost_gain;
};
struct player_value{
    float hp;
    float damage;
    float cooldown;
    float jump_force;
    float move_speed;
    int start_golds;
    int max_golds;
};
struct castle_value{
    float hp;
};
struct weapon_value{
    float damage_multi;       // 傷害倍率
    float cooldown_multi;     // 攻速倍率 (<1 更快, >1 更慢)
    float crit_chance;       // 暴擊機率
    float crit_multi;         // 暴擊倍率
    float crit_hp_percent;   // HP% 暴擊
    bool  piercing;          // 穿透
    float slow_percent;      // 緩速%
    float slow_duration;     // 緩速時間
    float freeze_duration;   // 冰凍時間
    float poison_multi;       // 毒傷倍率 (乘以 damage)
    float poison_interval;   // 毒間隔
    float splash_range;      // 範圍傷害範圍
    float splash_multi;       // 範圍傷害倍率 (乘以 damage)
};


static constexpr const char* WEAPON_NAME[] = {"Mud", "Arrow", "Stone Arrow", "Ice Slow", "Poison Arrow", "Piercing Arrow", "Iron Ball", "Fire Ball", "Rocket", "Ice Freeze", "Missile"};
static constexpr const char* POTION_NAME[] = {"Heal Player", "Heal Castle", "Attack", "Attack Speed", "Shield", "Move Speed", "Regenerate"};
static constexpr int WEAPON_COST[] = {0, 150, 150, 100, 100, 150, 100, 150, 100, 100, 150};  // 11 個
static constexpr int POTION_COST[] = {30, 30, 30, 30, 40, 20, 30};  // 7 個

//   Dmg*  Col*  CritChance  Crit*  CritHp%  Pierce  Slow%  SlowTime  FrezTime  Poison*  PoisonInterval  SplashRange  Splash*
static constexpr weapon_value WEAPON_VALUES[] = {
    {1.0f, 1.0f, 0.10f,      2.0f,  0.0f,    false,  0.1f, 1.0f,      0.0f,     0.0f,    0.0f,           0.0f,        0.0f},   // MUD
    {1.4f, 0.6f, 0.10f,      2.0f,  0.0f,    false,  0.0f,  0.0f,     0.0f,     0.0f,    0.0f,           0.0f,        0.0f},   // ARROW
    {1.7f, 1.0f, 0.10f,      2.0f,  0.0f,    false,  0.0f,  0.0f,     0.0f,     0.0f,    0.0f,           0.0f,        0.0f},   // STONE
    {1.2f, 0.6f, 0.10f,      2.0f,  0.0f,    false,  0.6f,  1.0f,     0.0f,     0.0f,    0.0f,           0.0f,        0.0f},   // ICE_SLOW
    {1.7f, 1.0f, 0.10f,      2.0f,  0.0f,    false,  0.0f,  0.0f,     0.0f,     0.2f,    1.0f,           0.0f,        0.0f},   // POISON
    {1.5f, 1.5f, 0.10f,      2.0f,  0.0f,    true,   0.0f,  0.0f,     0.0f,     0.0f,    0.0f,           0.0f,        0.0f},   // PIERCING_ARROW
    {2.5f, 1.0f, 0.03f,      1.0f,  0.9f,    false,  0.0f,  0.0f,     0.0f,     0.0f,    0.0f,           0.0f,        0.0f},   // IRON_BALL
    {1.8f, 1.0f, 0.10f,      2.0f,  0.0f,    false,  0.0f,  0.0f,     0.0f,     0.0f,    0.0f,           140.0f,       0.3f},   // FIRE_BALL
    {1.7f, 1.0f, 0.10f,      5.0f,  0.0f,    false,  0.0f,  0.0f,     0.0f,     0.0f,    0.0f,           0.0f,        0.0f},   // ROCKET
    {1.7f, 1.0f, 0.10f,      2.0f,  0.0f,    false,  0.0f,  0.0f,     1.0f,     0.0f,    0.0f,           0.0f,        0.0f},   // ICE_FREEZE
    {3.0f, 5.0f, 0.10f,      5.0f,  0.0f,    false,  0.0f,  0.0f,     0.0f,     0.0f,    0.0f,           200.0f,       1.0f},   // MISSILE
};
//                                             hp,damage, cooldown, jump_force, move_speed, start_golds, max_golds
static constexpr player_value PLAYER_VALUES = {200.0f, 20.0f, 0.5f, 800.0f, 200.0f, 200 ,200};
//                                             hp
static constexpr castle_value CASTLE_VALUES = {500.0f};
//       name         cost_base,cost_gain
static constexpr upgrade_value UPGRADE_VALUES[] = {
    {"[1] Attack +3",       20,  5},   // 攻擊力
    {"[2] Player HP +50",   20,  5},   // 玩家血量
    {"[3] Castle HP +50",   20,  5},   // 城堡血量
    {"[4] Max Gold +50",    50,  20},   // 金幣上限
    {"[5] Attack Speed",    80,  20},   // 攻速
    {"[6] Multi Shot +1",   150, 100},   // 多重射擊
    {"[7] Move SpeedUp",    50,  50},   // 移動速度
    {"[8] Crit Rate +2%",   30,  50},   // 暴擊率
    {"[9] Crit Damage +25%",30,  50},   // 暴擊傷害
};

//    hp       speed      reward   JumpMin    JumpMax       JumpCd(÷100)  FlyAmp     fly_spd(÷100)
static constexpr enemy_value ENEMY_VALUES[] = {
    { 70.0f,   {150,200}, {1,4},   {500,650},  {0,0},       {145,155},   {0,0},      {0,0}       },  // SLIMEGREEN
    { 500.0f,  {50,80},   {6,15},  {0,0},      {0,0},       {0,0},       {0,0},      {0,0}       },  // SLIMEBLACK
    { 40.0f,   {300,500}, {1,4},   {700,900},  {0,0},       {95,145},    {0,0},      {0,0}       },  // SLIMERED
    { 200.0f,  {110,150}, {6,15},  {750,850},  {1200,1400}, {135,155},   {0,0},      {0,0}       },  // SLIMEPURPLE
    { 150.0f,  {80,110},  {6,15},  {0,0},      {0,0},       {0,0},       {0,0},      {0,0}       },  // SLIMEBLUE
    { 170.0f,  {80,110},  {5,10},  {0,0},      {0,0},       {0,0},       {100,160},  {200,250}   },  // FLYINGANGEL
    { 40.0f,   {160,200}, {1,3},   {0,0},      {0,0},       {0,0},       {100,160},  {300,500}   },  // FLYINGBIRD
    { 1000.0f, {40,60},   {60,100},{0,0},      {0,0},       {0,0},       {100,120},  {100,150}   },  // FLYINGDRAGON
};

class game_factory {
    public:
        static constexpr float GROUND_Y = 765;
        // player(位置, 大小, 活著, 血量, 速度, 攻擊冷卻, 重力, 跳躍力, 地面Y, 移動速度)
        static player create_player(Vector2 position) {
            return player(position, {72,96}, true, PLAYER_VALUES.hp, {0,0}, PLAYER_VALUES.cooldown, 1600.0f, PLAYER_VALUES.jump_force, GROUND_Y-96, PLAYER_VALUES.move_speed);
        }
        // castle(位置, 大小, 活著, 血量)
        static castle create_castle(Vector2 position) {
            return castle(position, {200,700}, true, CASTLE_VALUES.hp);
        }
        // projectile(位置, 大小, 活著, 傷害, 速度)
        static projectile create_projectile(Vector2 position, float damage, Vector2 speed) {
            return projectile(position, {10,10}, true, damage, speed);
        }
        // 商店哥布林
        static goblin* create_goblin(Vector2 position){
            return new goblin(position, {62, 80}, true, 500.0f, {-150.0f, 0}, GetRandomValue(10, 40));
        }


        // enemy(位置, 碰撞箱大小, 活著, 血量, 速度, 目標X, 類型)
        // jump_behavior(地面Y, 小跳力道, 跳躍間隔)
        // jump_behavior(地面Y, 小跳力道, 大跳力道, 跳躍間隔)
        // buff_behavior(範圍, 減傷倍率, 增傷倍率, 加速倍率)
        // flying_enemy(位置, 碰撞箱大小, 活著, 血量, 速度, 目標X, 類型, 飄動幅度, 飄動速度)

        // 綠色（小跳）
        static enemy* create_enemy_green(Vector2 position, float hp_multiplier) {
            const auto& v = ENEMY_VALUES[SLIMEGREEN];
            enemy* e = new enemy(position, {60,54}, true, v.hp*hp_multiplier, {-(float)GetRandomValue((int)v.speed.x,(int)v.speed.y),0}, 0, SLIMEGREEN, GetRandomValue((int)v.reward.x,(int)v.reward.y));
            if(v.jump_small.x > 0){
                if(v.jump_big.x > 0) e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue((int)v.jump_small.x,(int)v.jump_small.y), (float)GetRandomValue((int)v.jump_big.x,(int)v.jump_big.y), (float)GetRandomValue((int)v.jump_cooldown.x,(int)v.jump_cooldown.y)/100.0f));
                else e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue((int)v.jump_small.x,(int)v.jump_small.y), (float)GetRandomValue((int)v.jump_cooldown.x,(int)v.jump_cooldown.y)/100.0f));
            }
            return e;
        }
        // 黑色（不跳）
        static enemy* create_enemy_black(Vector2 position, float hp_multiplier) {
            const auto& v = ENEMY_VALUES[SLIMEBLACK];
            return new enemy(position, {94,88}, true, v.hp*hp_multiplier, {-(float)GetRandomValue((int)v.speed.x,(int)v.speed.y),0}, 0, SLIMEBLACK, GetRandomValue((int)v.reward.x,(int)v.reward.y));
        }
        // 紅色（小跳）
        static enemy* create_enemy_red(Vector2 position, float hp_multiplier) {
            const auto& v = ENEMY_VALUES[SLIMERED];
            enemy* e = new enemy(position, {67,60}, true, v.hp*hp_multiplier, {-(float)GetRandomValue((int)v.speed.x,(int)v.speed.y),0}, 0, SLIMERED, GetRandomValue((int)v.reward.x,(int)v.reward.y));
            if(v.jump_small.x > 0){
                if(v.jump_big.x > 0) e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue((int)v.jump_small.x,(int)v.jump_small.y), (float)GetRandomValue((int)v.jump_big.x,(int)v.jump_big.y), (float)GetRandomValue((int)v.jump_cooldown.x,(int)v.jump_cooldown.y)/100.0f));
                else e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue((int)v.jump_small.x,(int)v.jump_small.y), (float)GetRandomValue((int)v.jump_cooldown.x,(int)v.jump_cooldown.y)/100.0f));
            }
            return e;
        }
        // 紫色（小跳+大跳交替）
        static enemy* create_enemy_purple(Vector2 position, float hp_multiplier) {
            const auto& v = ENEMY_VALUES[SLIMEPURPLE];
            enemy* e = new enemy(position, {63,61}, true, v.hp*hp_multiplier, {-(float)GetRandomValue((int)v.speed.x,(int)v.speed.y),0}, 0, SLIMEPURPLE, GetRandomValue((int)v.reward.x,(int)v.reward.y));
            if(v.jump_small.x > 0){
                if(v.jump_big.x > 0) e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue((int)v.jump_small.x,(int)v.jump_small.y), (float)GetRandomValue((int)v.jump_big.x,(int)v.jump_big.y), (float)GetRandomValue((int)v.jump_cooldown.x,(int)v.jump_cooldown.y)/100.0f));
                else e->add_behavior(new jump_behavior(position.y, (float)GetRandomValue((int)v.jump_small.x,(int)v.jump_small.y), (float)GetRandomValue((int)v.jump_cooldown.x,(int)v.jump_cooldown.y)/100.0f));
            }
            return e;
        }
        // 藍色（buff）
        static enemy* create_enemy_blue(Vector2 position, float hp_multiplier) {
            const auto& v = ENEMY_VALUES[SLIMEBLUE];
            enemy* e = new enemy(position, {61,54}, true, v.hp*hp_multiplier, {-(float)GetRandomValue((int)v.speed.x,(int)v.speed.y),0}, 0, SLIMEBLUE, GetRandomValue((int)v.reward.x,(int)v.reward.y));
            e->add_behavior(new buff_behavior(385, 0.5f, 3.0f, 1.1f));
            return e;
        }
        // 天使
        static flying_enemy* create_enemy_angel(Vector2 position, float hp_multiplier) {
            const auto& v = ENEMY_VALUES[FLYINGANGEL];
            flying_enemy* e = new flying_enemy(position, {88,81}, true, v.hp*hp_multiplier, {-(float)GetRandomValue((int)v.speed.x,(int)v.speed.y),0}, 0, FLYINGANGEL, (float)GetRandomValue((int)v.fly_amplitude.x,(int)v.fly_amplitude.y),(float)GetRandomValue((int)v.fly_speed.x,(int)v.fly_speed.y)/100.0f,GetRandomValue((int)v.reward.x,(int)v.reward.y));
            e->add_behavior(new heal_behavior(5.0f*hp_multiplier,0.5f,200));
            return e;
        }
        // 小鳥
        static flying_enemy* create_enemy_bird(Vector2 position, float hp_multiplier) {
            const auto& v = ENEMY_VALUES[FLYINGBIRD];
            return new flying_enemy(position, {39,37}, true, v.hp*hp_multiplier, {-(float)GetRandomValue((int)v.speed.x,(int)v.speed.y),0}, 0, FLYINGBIRD, (float)GetRandomValue((int)v.fly_amplitude.x,(int)v.fly_amplitude.y),(float)GetRandomValue((int)v.fly_speed.x,(int)v.fly_speed.y)/100.0f,GetRandomValue((int)v.reward.x,(int)v.reward.y));
        }
        // 飛龍（Boss）
        static flying_enemy* create_enemy_dragon(Vector2 position, float hp_multiplier) {
            const auto& v = ENEMY_VALUES[FLYINGDRAGON];
            return new flying_enemy(position, {136,136}, true, v.hp*hp_multiplier, {-(float)GetRandomValue((int)v.speed.x,(int)v.speed.y),0}, 0, FLYINGDRAGON, (float)GetRandomValue((int)v.fly_amplitude.x,(int)v.fly_amplitude.y),(float)GetRandomValue((int)v.fly_speed.x,(int)v.fly_speed.y)/100.0f,GetRandomValue((int)v.reward.x,(int)v.reward.y));
        } 

        // 武器效果
        static void apply_weapon(projectile& p, weapon_type type){
            const auto& w = WEAPON_VALUES[type];
            p.set_weapon_type(type);
            p.set_damage(p.get_damage() * w.damage_multi);
            p.set_cooldown_multiplier(w.cooldown_multi);
            p.set_crit_chance(w.crit_chance);
            p.set_crit_multiplier(w.crit_multi);
            p.set_crit_hp_percent(w.crit_hp_percent);
            p.set_piercing(w.piercing);
            p.set_slow_percent(w.slow_percent);
            p.set_slow_duration(w.slow_duration);
            p.set_freeze_duration(w.freeze_duration);
            if(w.poison_multi > 0){
                p.set_poison_damage(p.get_damage() * w.poison_multi);
                p.set_poison_interval(w.poison_interval);
            }
            if(w.splash_range > 0){
                p.set_splash_range(w.splash_range);
                p.set_splash_damage(p.get_damage() * w.splash_multi);
            }
        }
};