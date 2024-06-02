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
    int wave_id;
    int number_of_ENDPOINTS;
    int number_of_ENEMIES;
    float TIME_btw_SPAWN;
    std::vector<int> ENEMIES_type;
};