#include <iostream>
#include <filesystem>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <cmath>

// include
#include "Game.hpp"

// FONCTIONS PRINCIPALES

void Game::LOAD(TowerDefense &TD, std::string const &MAP_SCHEMA_ITD_path, int const &pixel_UNIT)
{
    TD.Load_All_Textures();
    TD.setup_MAP(MAP_SCHEMA_ITD_path, pixel_UNIT);
}
void Game::SETUP(TowerDefense &TD)
{
    glClearColor(0.0f, 0.0f, 0.24f, 1.0f); // Blue window
    TD.get_TOWERS_from_ITD();
    TD.get_ENEMIES_from_ITD();
    TD.get_WAVES_from_ITD();
    TD.get_SPRITE_SHEETS_from_ITD();
    TD.setup_WAVE();
    TD.get_ENEMIES_into_WAVE();
    TD.setup_ENEMIES_in_WAVE();
    TD.setup_SPRITE_SHEETS();
    TD.ui.setup_UI_Text();
}

void Game::UPDATE(TowerDefense &TD, const double &elapsedTime, const double &currentTime)
{
    if (TD.GAME_IS_PLAYING && !TD.PAUSE) // Si le jeu est lancé et on est pas en pause
    {
        TD.update_WAVE();
        TD.update_ENEMIES_in_WAVE(elapsedTime, currentTime);
        TD.update_TOWERS(elapsedTime, currentTime);
    }
}
void Game::RENDER(TowerDefense &TD, int &_width, int &_height)
{
    if (TD.GAME_IS_PLAYING)
    {
        draw_MAP_background(TD.LoadedTextures["images/textures/Map/BACKGROUND.png"], TD.map);
        TD.render_MAP();

        if (!TD.PAUSE)
        {
            TD.ui.PLAY_PAUSE.Label("PRESS -SPACE- TO PAUSE", _width / 2, 150, SimpleText::CENTER);
            TD.render_ENEMIES_in_WAVE();
            TD.render_TOWERS();
            TD.active_UI(_width, _height);
        }
        else
        {
            TD.ui.PLAY_PAUSE.Label("> PAUSE <", _width / 2, _height / 2, SimpleText::CENTER);
            TD.ui.PLAY_PAUSE.Label("PRESS -SPACE- TO PLAY", _width / 2, 150, SimpleText::CENTER);
            draw_BREAK_MENU(TD.map);
        }
    }
    else
    {
        TD.ui.PLAY_PAUSE.Label("PRESS > S < TO START", _width / 2, _height / 2, SimpleText::CENTER);
    }

    TD.ui.show_MAIN_TITLE(_width, _height);
    TD.ui.PLAY_PAUSE.Render();
}

void Game::active_KEY_CALLBACK(TowerDefense &TD, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        TD.GAME_IS_PLAYING = true;
    if (TD.GAME_IS_PLAYING)
    {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            TD.PAUSE = !TD.PAUSE;

        if (!TD.PAUSE)
        {
            if (key == GLFW_KEY_F && action == GLFW_PRESS)
            {
                for (auto &enemy : TD.current_ENEMIES_in_WAVE)
                    enemy.second.is_burning = !enemy.second.is_burning;
            }

            if (key == GLFW_KEY_Q && action == GLFW_PRESS)
                TD.ui.SHOW_TARGETED_CELL = !TD.ui.SHOW_TARGETED_CELL;

            if ((action == GLFW_PRESS || action == GLFW_REPEAT) && TD.ui.SHOW_TARGETED_CELL)
            {
                float top_neighbour{TD.ui.CELL_pos.y + 1};
                float bottom_neighbour{TD.ui.CELL_pos.y - 1};
                float right_neighbour{TD.ui.CELL_pos.x + 1};
                float left_neighbour{TD.ui.CELL_pos.x - 1};
                if (key == GLFW_KEY_UP && is_inside_MAP(TD.ui.CELL_pos.x, top_neighbour, TD.map))
                    TD.ui.CELL_pos.y++;
                if (key == GLFW_KEY_DOWN && is_inside_MAP(TD.ui.CELL_pos.x, bottom_neighbour, TD.map))
                    TD.ui.CELL_pos.y--;
                if (key == GLFW_KEY_LEFT && is_inside_MAP(left_neighbour, TD.ui.CELL_pos.y, TD.map))
                    TD.ui.CELL_pos.x--;
                if (key == GLFW_KEY_RIGHT && is_inside_MAP(right_neighbour, TD.ui.CELL_pos.y, TD.map))
                    TD.ui.CELL_pos.x++;
            }
        }
    }
}
