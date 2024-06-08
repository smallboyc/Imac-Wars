#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Graph.hpp"

void draw_MAP_background(GLuint const &texture, Map &map);
void draw_quad_with_texture(GLuint const &texture, float &x, float &y, Map &map);
void draw_enemy(GLuint const &texture, float &x, float &y, Map &map, float health);
void draw_tower(GLuint const &texture, float &x, float &y, Map &map);
void draw_quad(float &x, float &y, Map &map);
void draw_BREAK_MENU(Map &map);
void drawOrigin();