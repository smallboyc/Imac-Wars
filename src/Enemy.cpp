#include <glad/glad.h>
#include <simpletext.h>
#include <vector>
#include <GLFW/glfw3.h>
#include "Enemy.hpp"
#include "Map.hpp"
#include "utils.hpp"

// Dessine l'ennemi sur une map
void Enemy::draw(Map &map)
{
    draw_quad_with_texture(this->texture, this->pos.x, this->pos.y, map);
}

// void Enemy::move(Map &map)
// {
  

//     this->draw(map);
// }