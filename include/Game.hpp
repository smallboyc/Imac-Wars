#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include <vector>

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
        Map map;
        UI ui;
        int current_WAVE_id{0};
        Wave current_WAVE;                              // Vague actuelle
        std::vector<int> WAVES_checked;                 // Permet de vérifier si une vague a été réalisée
        std::unordered_map<int, Enemy> current_ENEMIES; // stock les ennemis d'une vague

        // ITD
        std::unordered_map<int, Wave> WAVES_ITD;    // stock toutes les vagues
        std::unordered_map<int, Enemy> ENEMIES_ITD; // stock tous les types d'ennemis

        // METHODS //
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
