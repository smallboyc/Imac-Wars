#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <img/img.hpp>
#include <glad/glad.h>
#include <simpletext.h>
#include "utils.hpp"
#include "Map.hpp"
#include "Enemy.hpp"

struct UI
{
    glm::vec2 CELL_pos{0, 0};
    bool SHOW_TARGETED_CELL{false};
    float WALLET{100};
    void show_CELLS(Map &map);
    void show_ENEMY_PROPERTIES(int const &current_WAVE_id, std::unordered_map<int, Enemy> &current_ENEMIES_in_WAVE);
};