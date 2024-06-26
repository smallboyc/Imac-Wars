#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "SoundEngine.hpp"

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
    float age{0}; // Age de la tour => Elle meurt à un certain moment.
    double previousTimeTower{0};
    bool isDestroyed{false};
    bool can_be_Selected{false};
    bool hover{false};
    bool isSelectedinUI{false};
    bool showProperty{false};
    float closest_enemy_dist{100};
    bool lockedEnemy{false};
    Enemy* closest_enemy;
    void setup(std::unordered_map<std::string, SpriteSheet> &SPRITE_SHEETS_ITD, glm::vec2 const &pixel_pos);
    void update(TowerDefense *, const double &elapsedTime, const double &currentTime);
    void render(Map &map);
};