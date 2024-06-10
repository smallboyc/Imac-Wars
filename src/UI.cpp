#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <sstream>
#include <fstream>
#include <cmath>

#include "simpletext.h"
#include "GLHelpers.hpp"
#include "UI.hpp"
#include "Draw.hpp"

void UI::setup_UI_Text()
{

    // MAIN
    this->MAIN_TITLE.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::CYAN);
    this->MAIN_TITLE.SetTextSize(SimpleText::FontSize::SIZE_32);
    this->MAIN_TITLE.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->MAIN_TITLE.EnableBlending(true);

    // PLAY & PAUSE
    //  MAIN
    this->PLAY_PAUSE.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::CYAN);
    this->PLAY_PAUSE.SetTextSize(SimpleText::FontSize::SIZE_32);
    this->PLAY_PAUSE.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->PLAY_PAUSE.EnableBlending(true);

    // WALLET
    this->WALLET_indicator.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::MAGENTA);
    this->WALLET_indicator.SetTextSize(SimpleText::FontSize::SIZE_32);
    this->WALLET_indicator.SetColorf(SimpleText::BACKGROUND_COLOR, 1.f, .8f, .0f, 0.9f);
    this->WALLET_indicator.EnableBlending(true);

    // WAVE
    this->WAVE_property.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    this->WAVE_property.SetTextSize(SimpleText::FontSize::SIZE_16);
    this->WAVE_property.SetColorf(SimpleText::BACKGROUND_COLOR, 0.5f, 0.f, 1.f, 0.5f);
    this->WAVE_property.EnableBlending(true);

    // ENEMY properties Title
    this->ENEMY_Title.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    this->ENEMY_Title.SetTextSize(SimpleText::FontSize::SIZE_16);
    this->ENEMY_Title.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 1.f, 1.f);
    this->ENEMY_Title.EnableBlending(true);

    // ENEMY PROPERTY
    this->ENEMY_property.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    this->ENEMY_property.SetTextSize(SimpleText::FontSize::SIZE_16);
    this->ENEMY_property.EnableBlending(true);
}

void UI::show_MAIN_TITLE(int &_width, int &_height)
{
    this->MAIN_TITLE.Label("- IMAC TOWER DEFENSE -", _width / 2, 100, SimpleText::CENTER);
    this->MAIN_TITLE.Render();
}

void UI::show_CELLS(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    if (this->SHOW_TARGETED_CELL)
    {
        for (Pixel &pixel : map.PIXELS)
        {
            if (pixel.on_Mouse_Over && pixel.is_VOID)
                draw_quad_with_texture(LoadedTextures["images/textures/Cursor/Cursor_set_Tower_ON.png"], pixel.x, pixel.y, map);
            else if (pixel.on_Mouse_Over && (pixel.is_PATH || pixel.is_TOWER))
                draw_quad_with_texture(LoadedTextures["images/textures/Cursor/Cursor_set_Tower_OFF.png"], pixel.x, pixel.y, map);
        }
        // draw_quad(this->CELL_pos.x, this->CELL_pos.y, map);
    }
}

void UI::show_WALLET(int &_width, int &_height)
{
    if (this->WALLET < 0)
        this->WALLET = 0;
    std::string WALLET_label{" Wallets : " + std::to_string(this->WALLET) + " "};
    this->WALLET_indicator.Label(WALLET_label.c_str(), _width / 2, _height - 100, SimpleText::CENTER);
    this->WALLET_indicator.Render();
}

void UI::show_ENEMY_PROPERTIES(int const &current_WAVE_id, std::unordered_map<int, Enemy> &current_ENEMIES_in_WAVE)
{

    std::string WAVE_label{"> VAGUE : " + std::to_string(current_WAVE_id) + " / " + " Ennemis restants : " + std::to_string(current_ENEMIES_in_WAVE.size()) + " "};
    this->WAVE_property.Label(WAVE_label.c_str(), 175, 250, SimpleText::LEFT);
    this->WAVE_property.Render();

    // ENEMY properties Title
    this->ENEMY_Title.Label("> ENEMY PROPERTIES <", 175, 300, SimpleText::LEFT);
    this->ENEMY_Title.Render();

    // ENEMY PROPERTY
    std::string text{};
    int gap{50};

    for (std::pair<int, Enemy> enemy : current_ENEMIES_in_WAVE)
    {
        if (enemy.second.isMoving)
            this->ENEMY_property.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 1.f, 0.f, 0.4f);
        else
            this->ENEMY_property.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 1.f);
        if (current_ENEMIES_in_WAVE.contains(enemy.first))
            text = "Enemy " + std::to_string(enemy.first) + " > " + "(" + enemy.second.name + ")" + " : X = " + std::to_string(static_cast<int>(std::round(enemy.second.pos.x))) + " & Y = " + std::to_string(static_cast<int>(std::round(enemy.second.pos.y)));
        this->ENEMY_property.Label(text.c_str(), 175, 300 + gap, SimpleText::LEFT);
        gap += 30;
    }
    this->ENEMY_property.Render();
}

void UI::show_TOWER_to_select(Map &map, Tower const &tower)
{
    draw_UI_ITEM(tower.texture, tower.UI_pos.x, tower.UI_pos.y, tower.UI_size, map);
}

void UI::show_CURSOR_select(Map &map, Tower &tower, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    GLuint texture;
    if (tower.hover) // Au survol de l'item de tour.
    {
        if (this->WALLET >= tower.price) // Si le joueur a assez d'argent.
        {
            tower.can_be_Selected = true;
            texture = LoadedTextures["images/textures/Cursor/Cursor_selected_ON.png"];
        }
        else
        {
            tower.can_be_Selected = false;
            texture = LoadedTextures["images/textures/Cursor/Cursor_selected_OFF.png"];
        }
        draw_UI_ITEM(texture, tower.UI_pos.x, tower.UI_pos.y, tower.UI_size, map);
    }
}