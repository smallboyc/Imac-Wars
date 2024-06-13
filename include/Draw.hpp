#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Graph.hpp"
#include "Enemy.hpp"
#include "Tower.hpp"
#include "SpriteSheet.hpp"
#include "Base.hpp"

void draw_WINDOW_Background(GLuint const &texture);
void draw_MAP_background(GLuint const &texture, Map &map);
void draw_quad_with_texture(GLuint const &texture, float &x, float &y, Map &map);
void draw_IMAC_WARS(GLuint const &texture, float &x, float &y, float &item_width, float &item_height, Map &map, const double &elapsedTime, const double &currentTime, float &previousTime);
void draw_UI_ITEM(GLuint const &texture, float const &x, float const &y, int const &item_width, int const &item_height, Map &map);
void draw_BASE_health(GLuint const &texture, Base &base, float const &x, float const &y, Map &map);
void draw_enemy(GLuint const &texture, Enemy &enemy, float &x, float &y, Map &map, float &health, float &hit);
void draw_tower(GLuint const &texture, Tower &tower, float &x, float &y, Map &map, float &health, float &age);
void draw_spriteSheet(SpriteSheet const &spriteSheet, float const &x, float const &y, Map &map);
void draw_quad(float &x, float &y, Map &map);
void draw_BREAK_MENU(Map &map);
void drawOrigin();