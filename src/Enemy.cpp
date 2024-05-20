#include <glad/glad.h>
#include <simpletext.h>
#include <vector>
#include <GLFW/glfw3.h>
#include "Enemy.hpp"
#include "Map.hpp"
#include "utils.hpp"

// Set l'ennemi : dans App::App()
void Enemy::set(Map &map)
{
    this->spawn.x = map.SHORTER_PATH[0].pixel.x;
    this->spawn.y = map.SHORTER_PATH[0].pixel.y;
    this->pos.x = this->spawn.x;
    this->pos.y = this->spawn.y;
    this->health = 0.1f;
    this->speed = 0.1f;
    this->travel = 0.0f;
    this->target_node_index = 1;
    // Chargement des textures au set
    this->textures["left"] = loadTexture(img::load(make_absolute_path("images/Enemy/enemy_left.png", true), 4, true));
    this->textures["right"] = loadTexture(img::load(make_absolute_path("images/Enemy/enemy_right.png", true), 4, true));
    this->textures["bottom"] = loadTexture(img::load(make_absolute_path("images/Enemy/enemy_bottom.png", true), 4, true));
    this->textures["top"] = loadTexture(img::load(make_absolute_path("images/Enemy/enemy_top.png", true), 4, true));
    this->texture = this->textures["right"]; // par défaut
}

// On fait avancer l'ennemi
void Enemy::move(Map &map)
{
    // target_node = le prochain noeud que l'on veut atteindre
    glm::vec2 target_node{map.SHORTER_PATH[this->target_node_index].pixel.x, map.SHORTER_PATH[this->target_node_index].pixel.y};
    float distance_x = (target_node.x - this->spawn.x) / map.NUMBER_OF_PIXELS_IN_LINE;
    float distance_y = (target_node.y - this->spawn.y) / map.NUMBER_OF_PIXELS_IN_LINE;

    // Mouvement positif / négatif pour x & y
    float step_x = (target_node.x > this->spawn.x) ? 1.0f : (target_node.x < this->spawn.x) ? -1.0f
                                                                                            : 0.0f;
    float step_y = (target_node.y > this->spawn.y) ? 1.0f : (target_node.y < this->spawn.y) ? -1.0f
                                                                                            : 0.0f;

    // Orientation des textures
    if (step_x == -1.0f)
        this->texture = this->textures["left"];
    else if (step_x == 1.0f)
        this->texture = this->textures["right"];
    else if (step_y == -1.0f)
        this->texture = this->textures["bottom"];
    else if (step_y == 1.0f)
        this->texture = this->textures["top"];

    if (abs(this->spawn.x - target_node.x) > abs(this->spawn.y - target_node.y))
    {
        if (this->travel <= abs(distance_x))
        {
            glTranslatef(step_x * this->travel, 0, 0);
            this->pos.x += step_x * (this->travel / map.NUMBER_OF_PIXELS_IN_LINE);
        }
        else
        {
            this->spawn.x = target_node.x;
            // std::cout << this->spawn.x << ":" << this->spawn.y << std::endl;
            this->travel = 0;
            this->target_node_index++;
        }
    }
    else
    {
        if (this->travel <= abs(distance_y))
        {
            glTranslatef(0, step_y * this->travel, 0);
            this->pos.y += step_y * (this->travel / map.NUMBER_OF_PIXELS_IN_LINE);
        }
        else
        {
            this->spawn.y = target_node.y;
            // std::cout << this->spawn.x << ":" << this->spawn.y << std::endl;
            this->travel = 0;
            this->target_node_index++;
        }
    }

    draw_enemy(this->texture, this->spawn.x, this->spawn.y, map, this->health);
}

// Mise à jour de l'état de l'ennemi
void Enemy::update_state(Map &map)
{
    // Si l'ennemi atteint sa cible = sacrifice
    if (this->spawn.x == map.SHORTER_PATH[map.SHORTER_PATH.size() - 1].pixel.x && this->spawn.y == map.SHORTER_PATH[map.SHORTER_PATH.size() - 1].pixel.y)
        this->isDead = true;
    // Si l'ennemi perd toute sa barre de vie
    if (this->health <= 0.01f)
        this->isDead = true;
}