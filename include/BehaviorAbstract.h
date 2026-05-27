#pragma once

class enemy;

// 行為抽象基底
class enemy_behavior {
public:
    virtual ~enemy_behavior() = default;
    virtual void apply(enemy& e, float dt) = 0;
};