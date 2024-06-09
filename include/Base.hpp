#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"

struct Base
{
    glm::vec2 pos;
    float health{10};
    float ouch{0};
    bool is_Destroyed{false};
};