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
        std::vector<Wave> Waves;
        std::vector<Enemy> Enemies;
        void setup_MAP();
        void render_MAP();
        void active_UI();
    };
}