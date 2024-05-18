#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"

struct Enemy
{
    glm::vec2 spawn;
    glm::vec2 pos;
    glm::vec2 speed;
    GLuint texture;
    bool isDead{false};
    void set(Map &map);
    void action(Map &map, float &i);
};