#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"
#include "Enemy.hpp"

struct Wave
{
    int level;
    std::vector<Enemy> ENEMIES;
    int ENEMIES_number;
    std::vector<std::pair<int, int>> out_in_GRAPH_points;
    void set_difficulty_from_ITD(int &level);
    // void get_WAVES_from_ITD();
    // void load_WAVE();
};