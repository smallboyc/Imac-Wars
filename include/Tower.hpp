#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"

struct Tower
{
    int type;
    glm::vec2 pos;
    GLuint texture;
    void setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);
    void update();
    void draw(Map& map);
};