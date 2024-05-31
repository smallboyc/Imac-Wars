#include <glad/glad.h>
#include <simpletext.h>
#include <vector>
#include <array>
#include <GLFW/glfw3.h>
#include "Enemy.hpp"
#include "Map.hpp"
#include "utils.hpp"
#include "SpriteSheet.hpp"

// Set l'ennemi : Sur une map précise, avec un type de chemin à parcourir, et un tableau de textures loadées
void Enemy::set(Map &map, int const &path, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    this->path = path;
    this->health = 0.07f;
    this->current.x = map.SHORTER_PATH_LIST[path][0].pixel.x;
    this->current.y = map.SHORTER_PATH_LIST[path][0].pixel.y;
    this->pos.x = this->current.x;
    this->pos.y = this->current.y;
    this->travel = 0.0f;
    this->target_node_index = 1;
    // Chargement des textures au set
    std::array<std::string, 4> coordinates = {"Left", "Right", "Bottom", "Top"};
    for (std::string coordinate : coordinates)
    {
        std::string path = "images/Enemy/" + this->name + "/" + this->name + "_" + coordinate + ".png";
        if (!LoadedTextures.contains(path))
            LoadedTextures[path] = loadTexture(img::load(make_absolute_path(path, true), 4, true));
        this->textures[coordinate] = LoadedTextures[path];
    }
}

// On fait avancer l'ennemi
void Enemy::move(Map &map)
{
    // target_node = le prochain noeud que l'on veut atteindre
    glm::vec2 target_node{map.SHORTER_PATH_LIST[this->path][this->target_node_index].pixel.x, map.SHORTER_PATH_LIST[this->path][this->target_node_index].pixel.y};
    float distance_x = (target_node.x - this->current.x) / map.NUMBER_OF_PIXELS_IN_LINE;
    float distance_y = (target_node.y - this->current.y) / map.NUMBER_OF_PIXELS_IN_LINE;

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
        if (this->travel <= abs(distance_x)) // tant que le trajet n'est pas terminé
        {
            glTranslatef(step_x * this->travel, 0, 0);                                       // déplacement selon des coordonnées normalisées (CN)
            this->pos.x += step_x * this->TIME * this->speed * map.NUMBER_OF_PIXELS_IN_LINE; // Position relative x
        }
        else // On est arrivé au noeud ciblé
        {
            this->current.x = target_node.x; // la cible atteinte devient notre nouvelle référence
            // std::cout << this->current.x << ":" << this->current.y << std::endl;
            this->travel = 0;          // le trajet est terminé, on en recommence un nouveau
            this->target_node_index++; // on change de cible
        }
    }
    else // parcours selon y
    {
        if (this->travel <= abs(distance_y)) // tant que le trajet n'est pas terminé
        {
            glTranslatef(0, step_y * this->travel, 0);
            this->pos.y += step_y * this->TIME * this->speed * map.NUMBER_OF_PIXELS_IN_LINE; // Position relative y
        }
        else // On est arrivé au noeud ciblé
        {
            this->current.y = target_node.y;
            // std::cout << this->current.x << ":" << this->current.y << std::endl;
            this->travel = 0;
            this->target_node_index++;
        }
    }
    draw_enemy(this->texture, this->current.x, this->current.y, map, this->health);
}

// Mise à jour de l'état de l'ennemi
void Enemy::update_state(Map &map, const double &elapsedTime)
{
    this->TIME = elapsedTime;

    // Mise à jour du trajet d'un noeud à l'autre
    this->travel += this->speed * this->TIME;

    // Si l'ennemi atteint sa cible = sacrifice
    if (this->current.x == map.SHORTER_PATH_LIST[0][map.SHORTER_PATH_LIST[0].size() - 1].pixel.x && this->current.y == map.SHORTER_PATH_LIST[0][map.SHORTER_PATH_LIST[0].size() - 1].pixel.y)
        this->isDead = true;
    // Si l'ennemi perd toute sa barre de vie
    if (this->health <= 0.01f)
        this->isDead = true;
}

void Enemy::display_position()
{
    std::cout << "(" << static_cast<int>(std::round(this->pos.x)) << " : " << static_cast<int>(std::round(this->pos.y)) << ")" << std::endl;
}