#include <glad/glad.h>
#include <simpletext.h>
#include <vector>
#include <array>
#include <GLFW/glfw3.h>
#include "Enemy.hpp"
#include "Map.hpp"
#include "utils.hpp"
#include "Draw.hpp"
#include "SpriteSheet.hpp"

// Set l'ennemi : Sur une map précise, avec un type de chemin à parcourir, et un tableau de textures loadées
void Enemy::setup(Map &map, int const &path, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    this->path = path;
    this->current.x = map.SHORTER_PATH_LIST[path][0].pixel.x;
    this->current.y = map.SHORTER_PATH_LIST[path][0].pixel.y;
    this->pos = this->current;
    this->travel = 0.0f;
    this->target_node_index = 1;
    // Chargement des textures au set
    std::array<std::string, 4> coordinates = {"Left", "Right", "Bottom", "Top"};
    for (std::string coordinate : coordinates)
    {
        std::string path = "images/textures/Enemy/" + this->name + "_" + coordinate + ".png";
        this->textures[coordinate] = LoadedTextures[path];
    }
}

void Enemy::update(const double &elapsedTime)
{
    // On récupère elapsedTime pour que le temps soit passé au render.
    this->TIME = elapsedTime;
}

// On fait avancer l'ennemi
void Enemy::render(Map &map)
{
    // target_node = le prochain noeud que l'on veut atteindre
    glm::vec2 target_node{map.SHORTER_PATH_LIST[this->path][this->target_node_index].pixel.x, map.SHORTER_PATH_LIST[this->path][this->target_node_index].pixel.y};

    // Mouvement positif / négatif pour x & y
    float step_x = (target_node.x > this->current.x) ? 1.0f : (target_node.x < this->current.x) ? -1.0f
                                                                                                : 0.0f;
    float step_y = (target_node.y > this->current.y) ? 1.0f : (target_node.y < this->current.y) ? -1.0f
                                                                                                : 0.0f;

    // Orientation des textures
    if (step_x == -1.0f)
        this->texture = this->textures["Left"];
    else if (step_x == 1.0f)
        this->texture = this->textures["Right"];
    else if (step_y == -1.0f)
        this->texture = this->textures["Bottom"];
    else if (step_y == 1.0f)
        this->texture = this->textures["Top"];

    if (abs(this->current.x - target_node.x) > abs(this->current.y - target_node.y)) // parcours selon x
    {

        if (std::round(this->pos.x * 10) / 10 == target_node.x) // si la pos du current = target
        {
            this->current.x = target_node.x;
            this->target_node_index++;
        }
        else
            this->pos.x += step_x * this->TIME * this->speed; // Position relative x
    }
    else // parcours selon y
    {
        if (std::round(this->pos.y * 10) / 10 == target_node.y) // si la pos du current = target
        {
            this->current.y = target_node.y;
            this->target_node_index++;
        }
        else
            this->pos.y += step_y * this->TIME * this->speed; // Position relative y
    }

    // Check si l'ennemi est sur un chemin en parcourant.
    if (!is_ENEMY_travel_correctly(*this, map))
    {
        std::cout << "Le chemin n'est pas continue." << std::endl;
        exit(EXIT_FAILURE);
    }

    draw_enemy(this->texture, *this, this->pos.x, this->pos.y, map, this->health, this->hit);
}
