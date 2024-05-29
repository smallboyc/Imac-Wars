#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <sstream>
#include <fstream>
#include <cmath>

#include "simpletext.h"
#include "Map.hpp"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include "UI.hpp"
#include "Enemy.hpp"

void UI::show_CELLS(Map &map)
{
    if (this->SHOW_TARGETED_CELL)
        draw_quad(this->CELL_pos.x, this->CELL_pos.y, map);
}

void UI::show_ENEMY_VITALS(std::unordered_map<int, Enemy> &current_ENEMIES_in_WAVE, Map &map)
{
    SimpleText enemy_property{};
    std::string text{};
    int gap{0};
    for (std::pair<int, Enemy> enemy : current_ENEMIES_in_WAVE)
    {
        if (current_ENEMIES_in_WAVE.contains(enemy.first))
            text = "Enemy " + std::to_string(enemy.first) + " > " + "(" + enemy.second.name + ")" + "X = " + std::to_string(static_cast<int>(std::round(enemy.second.pos.x))) + " & Y = " + std::to_string(static_cast<int>(std::round(enemy.second.pos.y)));
        enemy_property.Label(text.c_str(), 1280 / 8, 1280 / 2 + gap, SimpleText::LEFT);
        gap += 40;
    }
    enemy_property.Render();
}