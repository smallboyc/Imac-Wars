#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"

struct Enemy
{
    int type;
    std::string name;
    glm::vec2 current; // position actuelle de l'ennemi
    glm::vec2 pos;     // position relative de l'ennemi
    float health;      // Point de vie
    float hit{0};      // Nombre de fois que l'ennemi est touché par une tour
    float speed;       // Vitesse
    float damage;      // Dégats
    bool isMoving{false};
    bool is_burning{false};
    bool isDead{false};
    bool isTarget{true};
    float reSpeed{.0f};
    bool reSpeedSet{false};
    bool hasReachTarget{false};
    bool showProperty{false};
    int path;              // Chemin suivi par l'ennemi = id (déterminé par le plus court chemin)
    float travel;          // Trajet parcouru d'un noeud à l'autre => utilise elapsedTime
    float TIME;            // elapsedTime
    int target_node_index; // Index du prochain noeud à atteindre
    GLuint texture;
    std::unordered_map<std::string, GLuint> textures; // Contient les 4 textures de l'ennemi
    void setup(Map &map, int const &path, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);
    void update(const double &elapsedTime);
    void render(Map &map);
};