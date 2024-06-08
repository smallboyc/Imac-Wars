#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"

struct TowerDefense;

struct Tower
{
    int type;
    glm::vec2 pos;
    GLuint texture;
    float fireRate{3};
    Bullet bullet;
    void setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);
    void update(const double& elapsedTime, const double &currentTime, TowerDefense*);
    void draw(Map& map);
};