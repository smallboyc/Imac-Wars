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
    float travel;
    int target_node_index;
    GLuint texture;
    std::unordered_map<std::string, GLuint> textures;
    bool isDead{false};
    void set(Map &map);
    void move(Map &map);
    void update_state(Map &map);
};