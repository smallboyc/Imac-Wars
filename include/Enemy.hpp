#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"

struct Enemy
{
    glm::vec2 pos{0, 0};
    GLuint texture;
    bool isDead{false};
    void draw(Map &map);
};