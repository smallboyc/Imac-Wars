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
#include "SoundEngine.hpp"

void UI::get_TIME_in_UI(const double &elapsedTime, const double &currentTime)
{
    this->UI_elapsedTime = elapsedTime;
    this->UI_currentTime = currentTime;
}

void UI::setup_UI_Text()
{
    // MAIN
    this->MAIN_TITLE.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::YELLOW);
    this->MAIN_TITLE.SetTextSize(SimpleText::FontSize::SIZE_32);
    this->MAIN_TITLE.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->MAIN_TITLE.EnableBlending(true);

    // WAVE FINISHED
    this->WAVE_FINISHED.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::CYAN);
    this->WAVE_FINISHED.SetTextSize(SimpleText::FontSize::SIZE_64);
    this->WAVE_FINISHED.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->WAVE_FINISHED.EnableBlending(true);

    // PLAY & PAUSE
    this->PLAY_PAUSE.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::CYAN);
    this->PLAY_PAUSE.SetTextSize(SimpleText::FontSize::SIZE_32);
    this->PLAY_PAUSE.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->PLAY_PAUSE.EnableBlending(true);

    // PLAYER_WIN
    this->PLAYER_WIN.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::GREEN);
    this->PLAYER_WIN.SetTextSize(SimpleText::FontSize::SIZE_64);
    this->PLAYER_WIN.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->PLAYER_WIN.EnableBlending(true);

    // GAME OVER
    this->GAME_OVER.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::RED);
    this->GAME_OVER.SetTextSize(SimpleText::FontSize::SIZE_64);
    this->GAME_OVER.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->GAME_OVER.EnableBlending(true);

    // QUIT GAME
    this->QUIT_GAME.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::YELLOW);
    this->QUIT_GAME.SetTextSize(SimpleText::FontSize::SIZE_32);
    this->QUIT_GAME.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->QUIT_GAME.EnableBlending(true);

    // WALLET
    this->WALLET_indicator.SetTextSize(SimpleText::FontSize::SIZE_32);
    this->WALLET_indicator.SetColorf(SimpleText::BACKGROUND_COLOR, .0f, .0f, .0f, 0.9f);
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

void UI::show_IMAC_WARS_TITLE(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    draw_IMAC_WARS(LoadedTextures["images/textures/Start/IMAC_WARS.png"], this->TITLE_pos.x, this->TITLE_pos.y, this->TITLE_size.x, this->TITLE_size.y, map, this->UI_elapsedTime, this->UI_currentTime, this->UI_previous_Time);
}

void UI::show_MAIN_TITLE(int &_width, int &_height)
{
    this->MAIN_TITLE.Label("- IMAC WARS : Tower Defense -", _width / 2, 100, SimpleText::CENTER);
    this->MAIN_TITLE.Render();
}

void UI::show_WAVE_FINISHED(int &_width, int &_height, size_t current_WAVE_id)
{
    std::string text = "> WAVE " + std::to_string(current_WAVE_id) + " FINISHED! <";

    this->WAVE_FINISHED.Label(text.c_str(), _width / 2, _height / 2, SimpleText::CENTER);
    this->WAVE_FINISHED.Label("PRESS -ENTER- TO CONTINUE", _width / 2, _height / 2 + 150, SimpleText::CENTER);
    this->WAVE_FINISHED.Render();
}

void UI::show_PLAYER_WIN(int &_width, int &_height)
{
    this->PLAYER_WIN.Label("CONGRATULATIONS, YOU WIN !", _width / 2, _height / 2, SimpleText::CENTER);
    this->PLAYER_WIN.Render();
}

void UI::show_GAME_OVER(int &_width, int &_height, std::unordered_map<int, Tower> current_TOWERS_in_MAP)
{
    this->GAME_OVER.Label("GAME OVER !", _width / 2, _height / 2, SimpleText::CENTER);
    if (current_TOWERS_in_MAP.empty() && this->WALLET == 0)
        this->GAME_OVER.Label("NO TOUR ON MAP & NO MONEY!", _width / 2, _height / 2 + 100, SimpleText::CENTER);
    else
        this->GAME_OVER.Label("DEATH STAR HAS BEEN DESTROYED...", _width / 2, _height / 2 + 100, SimpleText::CENTER);
    this->GAME_OVER.Render();
}

void UI::show_QUIT_GAME(int &_width, int &_height)
{
    this->QUIT_GAME.Label("Press Q to Quit the game!", _width / 2, _height - 100, SimpleText::CENTER);
    this->QUIT_GAME.Render();
}

void UI::show_CURSOR_on_MAP(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{

    for (Pixel &pixel : map.PIXELS)
    {
        if (pixel.on_Mouse_Over && pixel.is_VOID && !pixel.is_FORBIDDEN)
            draw_quad_with_texture(LoadedTextures["images/textures/Cursor/Cursor_set_Tower_ON.png"], pixel.x, pixel.y, map);
        else if (pixel.on_Mouse_Over && (pixel.is_PATH || pixel.is_TOWER || pixel.is_FORBIDDEN))
            draw_quad_with_texture(LoadedTextures["images/textures/Cursor/Cursor_set_Tower_OFF.png"], pixel.x, pixel.y, map);
    }
}

void UI::show_WALLET(int &_width, int &_height)
{
    if (this->WALLET < 0)
        this->WALLET = 0;

    if (this->WALLET == 0)
        this->WALLET_indicator.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::RED);
    else
        this->WALLET_indicator.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::MAGENTA);
    std::string WALLET_label{" ARGENT : " + std::to_string(this->WALLET) + " "};
    this->WALLET_indicator.Label(WALLET_label.c_str(), _width / 2, _height - 50, SimpleText::CENTER);
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

void UI::show_ENEMY_PROPERTY(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures, std::unordered_map<int, Enemy> &current_ENEMIES_in_WAVE)
{
    for (auto &enemy : current_ENEMIES_in_WAVE)
    {
        if (enemy.second.showProperty)
            draw_UI_ITEM(LoadedTextures["images/textures/Help/Help.png"], -10, 1.5, 9, 12, map);
    }
}

void UI::show_TOWER_to_select(Map &map, Tower const &tower)
{
    draw_UI_ITEM(tower.texture, tower.UI_pos.x, tower.UI_pos.y, tower.UI_size, tower.UI_size, map);
}

void UI::show_HELP_in_PAUSE(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    // Left menu
    draw_UI_ITEM(LoadedTextures["images/textures/Help/Help-Ennemies.png"], -10, 0.5, 9, 14, map);

    // Right menu
    draw_UI_ITEM(LoadedTextures["images/textures/Help/Help-Allies.png"], 16, 0.5, 9, 14, map);
}

void UI::show_CURSOR_select(Map &map, Tower &tower, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    GLuint texture;

    if (tower.hover || tower.isSelectedinUI) // Au survol de l'item de tour.
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
        draw_UI_ITEM(texture, tower.UI_pos.x, tower.UI_pos.y, tower.UI_size, tower.UI_size, map);
    }
}