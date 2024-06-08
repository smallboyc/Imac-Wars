#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"
#include "Enemy.hpp"

struct Bullet
{
    GLuint texture;
    glm::vec2 pos;
    glm::vec2 direction;
    bool fixedDirection{false};
    bool isBeingShot{false};
    void setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);
    void update(Enemy&);
    void draw(Map&);
};