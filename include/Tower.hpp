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
    float cadence;
    float degats;
    float portee;
    Bullet bullet;
    bool can_be_Selected{false};
    bool hover{false};
    void setup(std::unordered_map<std::string, SpriteSheet> &SPRITE_SHEETS_ITD, glm::vec2 const &pixel_pos);
    void update(TowerDefense *, const double &elapsedTime, const double &currentTime);
    void render(Map &map);
};