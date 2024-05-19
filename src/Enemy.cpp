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
    this->texture = loadTexture(img::load(make_absolute_path("images/Tiles/tile_0024.png", true), 4, true));
}

// On fait avancer l'ennemi
void Enemy::move(Map &map, float &i)
{
    static int target_index{1}; // on l'incrément à chaque appel => static ne le def qu'une fois.
    // target_node = le prochain noeud que l'on veut atteindre
    glm::vec2 target_node{map.SHORTER_PATH[target_index].pixel.x, map.SHORTER_PATH[target_index].pixel.y};
    float distance_x = (target_node.x - this->spawn.x) / map.NUMBER_OF_PIXELS_IN_LINE;
    float distance_y = (target_node.y - this->spawn.y) / map.NUMBER_OF_PIXELS_IN_LINE;

    // Mouvement positif / négatif pour x & y
    float step_x = (target_node.x > this->spawn.x) ? 1.0f : (target_node.x < this->spawn.x) ? -1.0f
                                                                                            : 0.0f;
    float step_y = (target_node.y > this->spawn.y) ? 1.0f : (target_node.y < this->spawn.y) ? -1.0f
                                                                                            : 0.0f;

    // Orientation des textures
    if (step_x == -1.0f) // gauche
        this->texture = loadTexture(img::load(make_absolute_path("images/Enemy/enemy_left.png", true), 4, true));
    else if (step_x == 1.0f) // droite
        this->texture = loadTexture(img::load(make_absolute_path("images/Enemy/enemy_right.png", true), 4, true));
    else if (step_y == -1.0f) // bas
        this->texture = loadTexture(img::load(make_absolute_path("images/Enemy/enemy_bottom.png", true), 4, true));
    else if (step_y == 1.0f) // haut
        this->texture = loadTexture(img::load(make_absolute_path("images/Enemy/enemy_top.png", true), 4, true));

    if (abs(this->spawn.x - target_node.x) > abs(this->spawn.y - target_node.y))
    {
        if (i <= abs(distance_x))
        {
            glTranslatef(step_x * i, 0, 0);
            this->pos.x += step_x * (i / map.NUMBER_OF_PIXELS_IN_LINE);
        }
        else
        {
            this->spawn.x = target_node.x;
            // std::cout << this->spawn.x << ":" << this->spawn.y << std::endl;
            i = 0;
            target_index++;
        }
    }
    else
    {
        if (i <= abs(distance_y))
        {
            glTranslatef(0, step_y * i, 0);
            this->pos.y += step_y * (i / map.NUMBER_OF_PIXELS_IN_LINE);
        }
        else
        {
            this->spawn.y = target_node.y;
            // std::cout << this->spawn.x << ":" << this->spawn.y << std::endl;
            i = 0;
            target_index++;
        }
    }

    draw_enemy(this->texture, this->spawn.x, this->spawn.y, map, this->health);
}

//Mise à jour de l'état de l'ennemi
void Enemy::update_state(Map &map)
{
    // Si l'ennemi atteint sa cible = sacrifice
    if (this->spawn.x == map.SHORTER_PATH[map.SHORTER_PATH.size() - 1].pixel.x && this->spawn.y == map.SHORTER_PATH[map.SHORTER_PATH.size() - 1].pixel.y)
        this->isDead = true;
    // Si l'ennemi perd toute sa barre de vie
    if (this->health <= 0.01f)
        this->isDead = true;
}