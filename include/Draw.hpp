#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Graph.hpp"
#include "Enemy.hpp"
#include "SpriteSheet.hpp"
#include "Base.hpp"

void draw_MAP_background(GLuint const &texture, Map &map);
void draw_quad_with_texture(GLuint const &texture, float &x, float &y, Map &map);
void draw_UI_ITEM(GLuint const &texture, float const &x, float const &y, int const &item_size, Map &map);
void draw_BASE_health(Base &base, float const &x, float const &y, Map &map);
void draw_enemy(GLuint const &texture, Enemy &enemy, float &x, float &y, Map &map, float &health, float &hit);
void draw_tower(GLuint const &texture, float &x, float &y, Map &map);
void draw_spriteSheet(SpriteSheet const &spriteSheet, float const &x, float const &y, Map &map);
void draw_quad(float &x, float &y, Map &map);
void draw_BREAK_MENU(Map &map);
void drawOrigin();