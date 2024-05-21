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
    int number_of_ENDPOINTS;
    int number_of_ENEMIES;
    int ENEMIES_number;
    std::vector<int> ENEMIES_type;
    void set_difficulty_from_ITD(int &level);
    // void load_WAVE();
};