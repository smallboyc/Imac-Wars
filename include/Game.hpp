#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include <vector>
#include <string>

// include
#include "TowerDefense.hpp"

namespace Game
{
    void LOAD(TowerDefense &TD, std::string const &MAP_SCHEMA_ITD_path, int const &pixel_UNIT);
    void SETUP(TowerDefense &TD);
    void UPDATE(TowerDefense &TD, const double &elapsedTime, const double &currentTime);
    void RENDER(TowerDefense &TD, int &_width, int &_height);

    // PLAYER CALLBACK
    void active_KEY_CALLBACK(TowerDefense &TD, int key, int scancode, int action, int mods);
}
