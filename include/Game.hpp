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

namespace Game
{
    struct TowerDefense
    {
        bool GAME_IS_PLAYING{false};
        bool PAUSE{false};
        Map map;
        UI ui;
        std::unordered_map<std::filesystem::path, GLuint> LoadedTextures; // stock toutes les instances de textures load (1 fois)
        // Vagues
        size_t current_WAVE_id{0};               // ID de la vague actuelle
        Wave current_WAVE;                       // Vague actuelle
        std::vector<int> WAVES_checked;          // Permet de vérifier si une vague a été réalisée
        std::unordered_map<int, Wave> WAVES_ITD; // récupère toutes les vagues (depuis l'ITD)
        // Enemis
        std::unordered_map<int, Enemy> current_ENEMIES_in_WAVE; // stock les ennemis d'une vague
        std::unordered_map<int, Enemy> ENEMIES_ITD;             // récupère tous les types d'ennemis (depuis l)

        // MAP
        void setup_MAP();
        void render_MAP();
        // UI
        void active_UI();
        // ITD
        void get_WAVES_from_ITD();
        void get_ENEMIES_from_ITD();
        // WAVE
        void setup_WAVE();
        void get_ENEMIES_into_WAVE();
        void update_WAVE();
        // ENEMY
        void setup_ENEMIES();
        void update_ENEMIES(const double &elapsedTime);
        void render_ENEMIES();
    };

}
