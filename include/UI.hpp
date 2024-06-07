#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <img/img.hpp>
#include <glad/glad.h>
#include <simpletext.h>
#include "utils.hpp"
#include "Map.hpp"
#include "Enemy.hpp"

struct UI
{
    glm::vec2 CELL_pos{0, 0};
    bool SHOW_TARGETED_CELL{false};
    int WALLET{100};
    SimpleText MAIN_TITLE{};
    SimpleText PLAY_PAUSE{};
    SimpleText WALLET_indicator{};
    SimpleText WAVE_property{};
    SimpleText ENEMY_Title{};
    SimpleText ENEMY_property{};
    void setup_UI_Text();
    void show_MAIN_TITLE(int &_width, int &_height);
    void show_CELLS(Map &map);
    void show_WALLET(int &_width, int &_height);
    void show_ENEMY_PROPERTIES(int const &current_WAVE_id, std::unordered_map<int, Enemy> &current_ENEMIES_in_WAVE);
};