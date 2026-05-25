#pragma once
#include "raylib.h"
#include <string>

struct damage_text{
    Vector2 position;
    std::string text;
    float timer;
    float lifetime;
    Color color;
};
