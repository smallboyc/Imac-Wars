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
    glm::vec2 UI_pos;
    int UI_size;
    int price;
    float health;
    GLuint texture;
    float fireRate{3};
    Bullet bullet;
    bool can_be_Selected{false};
    bool hover{false};
    void setup(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures, glm::vec2 const &pixel_pos);
    void update(TowerDefense *, const double &elapsedTime);
    void render(Map &map);
};