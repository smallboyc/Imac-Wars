#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"
#include "Enemy.hpp"
#include "SpriteSheet.hpp"

struct Bullet
{
    // GLuint texture;
    SpriteSheet sprite;
    glm::vec2 pos;
    glm::vec2 direction;
    bool fixedDirection{false};
    bool isBeingShot{false};
    bool hitEnemy{false};
    void setup(std::unordered_map<std::string, SpriteSheet> &SPRITE_SHEETS_ITD, glm::vec2 &tower_pos, int tower_id);
    void update(Enemy &, const double &elapsedTime, const double &currentTime, float degats);
    void render(Map &);
};