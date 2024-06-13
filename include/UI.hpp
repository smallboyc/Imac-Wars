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
#include "Tower.hpp"

struct UI
{
    // START
    glm::vec2 TITLE_pos{7, 7};
    glm::vec2 TITLE_size{1, 1};
    // IN GAME
    int WALLET{20};
    SimpleText MAIN_TITLE{};
    SimpleText WAVE_FINISHED{};
    SimpleText PLAY_PAUSE{};
    SimpleText PLAYER_WIN{};
    SimpleText GAME_OVER{};
    SimpleText QUIT_GAME{};
    SimpleText WALLET_indicator{};
    SimpleText WAVE_property{};
    SimpleText ENEMY_Title{};
    SimpleText ENEMY_property{};

    // TIMES
    double UI_elapsedTime;
    double UI_currentTime;
    float UI_previous_Time{0.f};
    void get_TIME_in_UI(const double &elapsedTime, const double &currentTime);

    // START IMAC WARs
    void
    show_IMAC_WARS_TITLE(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);

    // Textes
    void setup_UI_Text();
    void show_MAIN_TITLE(int &_width, int &_height);
    void show_WAVE_FINISHED(int &_width, int &_height, size_t current_WAVE_id);
    void show_PLAYER_WIN(int &_width, int &_height);
    void show_GAME_OVER(int &_width, int &_height, std::unordered_map<int, Tower> current_TOWERS_in_MAP);
    void show_QUIT_GAME(int &_width, int &_height);

    // Elements de jeu
    void show_CURSOR_on_MAP(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);
    void show_WALLET(int &_width, int &_height);
    void show_TOWER_to_select(Map &map, Tower const &tower);
    void show_HELP_in_PAUSE(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);
    void show_CURSOR_select(Map &map, Tower &tower, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures);
    void show_ENEMY_PROPERTIES(int const &current_WAVE_id, std::unordered_map<int, Enemy> &current_ENEMIES_in_WAVE);
    void show_ENEMY_PROPERTY(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures, std::unordered_map<int, Enemy> &current_ENEMIES_in_WAVE);
};