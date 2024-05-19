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
    float health;
    float speed;
    GLuint texture;
    bool isDead{false};
    void set(Map &map);
    void move(Map &map, float &i);
    void update_state(Map &map);
};