#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include <vector>
#include <string>

// include
#include "GLHelpers.hpp"
#include "Map.hpp"
#include "Enemy.hpp"
#include "Wave.hpp"
#include "UI.hpp"
#include "utils.hpp"
#include "Draw.hpp"
#include "SpriteSheet.hpp"
#include "Tower.hpp"

struct TowerDefense
{
    bool GAME_IS_PLAYING{false};
    bool PAUSE{false};
    bool FINISHED_WAVE{false};
    bool GAME_OVER{false};
    bool PLAYER_WIN{false};
    Map map;
    UI ui;
    Base base;
    std::unordered_map<std::filesystem::path, GLuint> LoadedTextures; // stock toutes les instances de textures load (1 fois)
    // Vagues
    size_t current_WAVE_id{0};               // ID de la vague actuelle
    std::unordered_map<int, Wave> WAVES_ITD; // récupère toutes les vagues (depuis l'ITD)
    Wave current_WAVE;                       // Vague actuelle
    std::vector<int> WAVES_checked;          // Permet de vérifier si une vague a été réalisée
    // Ennemis
    std::unordered_map<int, Enemy> current_ENEMIES_in_WAVE; // stock les ennemis d'une vague
    std::unordered_map<int, Enemy> ENEMIES_ITD;             // récupère tous les types d'ennemis (depuis l'ITD)
    int ENEMIES_id_to_launch{0};                            // Cible l'ennemi à afficher en premier
    double TIME_since_last_ENEMY_launched;                  // Temps écoulé entre chaque spawn d'ennemi
    // Towers
    std::unordered_map<int, Tower> current_TOWERS_in_MAP; // stock les tours qui sont sur la map
    std::unordered_map<int, Tower> TOWERS_ITD;            // récupère tous les types de tours (depuis l'ITD)
    int current_TOWER_id{1};                              // ID de la tour qui peut être sélectionné par le joueur
    int towerID{0};
    // Particules
    std::unordered_map<std::string, SpriteSheet> SPRITE_SHEETS_ITD; // stock tous les sprites sheets
    // TEXTURES
    void Load_All_Textures();
    // MAP
    void setup_MAP(std::string const path, int const pixels_in_LINE);
    void render_MAP();
    // BASE
    void setup_BASE();
    void render_BASE_health();
    // UI
    void active_UI(int &_width, int &_height);
    // ITD
    void get_TOWERS_from_ITD();
    void get_ENEMIES_from_ITD();
    void get_WAVES_from_ITD();
    void get_SPRITE_SHEETS_from_ITD();
    // WAVE
    void setup_WAVE();
    void get_ENEMIES_into_WAVE();
    void update_WAVE();
    // ENEMY
    void setup_ENEMIES_in_WAVE();
    void update_ENEMIES_in_WAVE(const double &elapsedTime, const double &currentTime);
    void render_ENEMIES_in_WAVE();
    // TOWER
    void update_TOWERS(const double &elapsedTime, const double &currentTime);
    void render_TOWERS();
    // SPRITE SHEETS
    void setup_SPRITE_SHEETS();
};