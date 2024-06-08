#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"

struct Bullet
{
    GLuint texture;
    void setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);
    void update();
    void draw(Map&);
};