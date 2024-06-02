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
#include "Draw.hpp"
#include "Enemy.hpp"

void UI::show_CELLS(Map &map)
{
    if (this->SHOW_TARGETED_CELL)
        draw_quad(this->CELL_pos.x, this->CELL_pos.y, map);
}

void UI::show_ENEMY_PROPERTIES(int const &current_WAVE_id, std::unordered_map<int, Enemy> &current_ENEMIES_in_WAVE)
{
    SimpleText WAVE_property{};
    WAVE_property.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    WAVE_property.SetTextSize(SimpleText::FontSize::SIZE_32);
    WAVE_property.SetColorf(SimpleText::BACKGROUND_COLOR, 0.5f, 0.f, 1.f, 0.2f);
    std::string WAVE_label{"> VAGUE : " + std::to_string(current_WAVE_id) + " / " + " Ennemis restants : "+ std::to_string(current_ENEMIES_in_WAVE.size()) + " "} ;
    WAVE_property.Label(WAVE_label.c_str(), 175, 250, SimpleText::LEFT);
    WAVE_property.Render();
    SimpleText enemy_property{};
    enemy_property.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    enemy_property.SetTextSize(SimpleText::FontSize::SIZE_32);
    enemy_property.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 1.f, 1.f);
    enemy_property.Label("> ENEMY PROPERTIES <", 175, 300, SimpleText::LEFT);

    enemy_property.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 1.f);

    enemy_property.EnableBlending(true);

    std::string text{};
    int gap{50};

    for (std::pair<int, Enemy> enemy : current_ENEMIES_in_WAVE)
    {
        if (enemy.second.isMoving)
            enemy_property.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 1.f, 0.f, 0.4f);
        if (current_ENEMIES_in_WAVE.contains(enemy.first))
            text = "Enemy " + std::to_string(enemy.first) + " > " + "(" + enemy.second.name + ")" + " : X = " + std::to_string(static_cast<int>(std::round(enemy.second.pos.x))) + " & Y = " + std::to_string(static_cast<int>(std::round(enemy.second.pos.y)));
        enemy_property.Label(text.c_str(), 175, 300 + gap, SimpleText::LEFT);
        gap += 50;
    }
    enemy_property.Render();
}
