#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>

struct Enemy
{
    glm::vec2 pos;
    std::vector<GLuint> texture;
}