#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include <vector>
#include <string>

// include
#include "TowerDefense.hpp"
#include "SoundEngine.hpp"

namespace Game
{
  

    void LOAD(TowerDefense &TD);
    void SETUP(TowerDefense &TD, std::string const &MAP_SCHEMA_ITD_path, int const &pixel_UNIT);
    void UPDATE(TowerDefense &TD, const double &elapsedTime, const double &currentTime);
    void RENDER(TowerDefense &TD, int &_width, int &_height);

    // PLAYER CALLBACK
    void active_KEY_CALLBACK(TowerDefense &TD, int key, int scancode, int action, int mods);
    void active_MOUSE_CLICK_CALLBACK(TowerDefense &TD, GLFWwindow *window, float &_viewSize, int button, int action, int mods);
    void active_MOUSE_POSITION_CALLBACK(TowerDefense &TD, GLFWwindow *window, float &_viewSize, double xpos, double ypos);
}
