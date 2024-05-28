#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "Map.hpp"
#include "utils.hpp"
#include <vector>
#include "GLHelpers.hpp"

struct Enemy
{
    int type{0};
    std::string name;
    glm::vec2 current;     // position actuel de l'ennemi
    glm::vec2 pos;         // position relative de l'ennemi
    float health;          // Point de vie
    float speed;           // Vitesse
    float damage;          // Dégats
    int path{0};           // Chemin suivi par l'ennemi = id (déterminé par le plus court chemin)
    float travel;          // Trajet parcouru d'un noeud à l'autre => utilise elapsedTime
    float TIME;            // elapsedTime
    int target_node_index; // Index du prochain noeud à atteindre
    GLuint texture;
    std::unordered_map<std::string, GLuint> textures; // Contient les 4 textures de l'ennemi
    bool isDead{false};
    void set(Map &map, int const &path, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);
    void move(Map &map);
    void update_state(Map &map, const double &elapsedTime);

    // Debug
    void display_position();
};