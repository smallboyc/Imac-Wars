#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"
#include "Enemy.hpp"
#include "SpriteSheet.hpp"

struct Tower;

struct Bullet
{
    // GLuint texture;
    SpriteSheet sprite;
    glm::vec2 pos;
    glm::vec2 direction;
    bool fixedDirection{false};
    bool isBeingShot{false};
    bool hitEnemy{false};
    void setup(std::unordered_map<std::string, SpriteSheet> &SPRITE_SHEETS_ITD, Tower*);
    void update(Enemy&, const double &elapsedTime, const double &currentTime, Tower*);
    void render(Map &);
};