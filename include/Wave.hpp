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
    int number_of_ENDPOINTS;
    int number_of_ENEMIES;
    std::vector<int> ENEMIES_type;
};