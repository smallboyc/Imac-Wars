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

    // START BUTTON
    this->START_GAME.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::CYAN);
    this->START_GAME.SetTextSize(SimpleText::FontSize::SIZE_32);
    this->START_GAME.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->START_GAME.EnableBlending(true);

    // WAVE FINISHED
    this->WAVE_FINISHED.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::CYAN);
    this->WAVE_FINISHED.SetTextSize(SimpleText::FontSize::SIZE_32);
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
    draw_IMAC_WARS(LoadedTextures["images/textures/Start/IMAC_WARS_02.png"], this->TITLE_pos.x, this->TITLE_pos.y, this->TITLE_size.x, this->TITLE_size.y, map, this->UI_elapsedTime, this->UI_currentTime, this->UI_previous_Time);
}

void UI::show_PLAY_button(int &_width, int &_height, Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    draw_UI_ITEM(LoadedTextures["images/textures/Other/Start_Button.png"], 4.5, -2, 6, 2, map);

    if (std::round(std::fmod(this->UI_currentTime, 2)) == 1)
        this->START_GAME.Label("PRESS > S <", _width / 2, _height - 20, SimpleText::CENTER);

    this->START_GAME.Render();
}

void UI::show_MAIN_TITLE(int &_width, int &_height)
{
    this->MAIN_TITLE.Label("- IMAC WARS : Tower Defense -", _width / 2, 100, SimpleText::CENTER);
    this->MAIN_TITLE.Render();
}

void UI::show_WAVE_FINISHED(Map &map, int &_width, int &_height, size_t current_WAVE_id, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    // std::string text = "> WAVE " + std::to_string(current_WAVE_id) + " FINISHED! <";

    draw_UI_ITEM(LoadedTextures["images/textures/Other/Wave_Finished_Pannel.png"], 4, 6, 7, 3, map);
    // this->WAVE_FINISHED.Label(text.c_str(), _width / 2, _height / 2, SimpleText::CENTER);
    this->WAVE_FINISHED.Label("PRESS -ENTER- TO CONTINUE", _width / 2, _height / 2 + 150, SimpleText::CENTER);
    this->WAVE_FINISHED.Render();
}

void UI::show_PLAYER_WIN(int &_width, int &_height)
{
    this->PLAYER_WIN.Label("CONGRATULATIONS, YOU WIN !", _width / 2, _height / 2, SimpleText::CENTER);
    this->PLAYER_WIN.Label("Press > A < to continue!", _width / 2, _height / 2 + 200, SimpleText::CENTER);
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
    this->QUIT_GAME.Label("Press Q to Quit the game!", _width / 2, _height - 80, SimpleText::CENTER);
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

void UI::show_WALLET(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    if (this->WALLET < 0)
        this->WALLET = 0;
    else if (this->WALLET > 999)
        this->WALLET = 999;
    glPushMatrix();
    glTranslatef(1.3, 1.05, 0);
    draw_UI_ITEM(LoadedTextures["images/textures/Wallet/Wallet_Indicator.png"], 0, 0, 6, 3, map);
    glPushMatrix();
    glTranslatef(-0.13, 0.02, 0);
    draw_UI_ITEM(LoadedTextures["images/textures/Other/Money_02.png"], 0, 0, 2, 2, map);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.08, 0.06, 0);
    std::string WALLET_string = std::to_string(this->WALLET);
    if (WALLET_string.size() == 1)
    {
        draw_UI_ITEM(LoadedTextures["images/textures/Numbers/0.png"], 0, 0, 1, 1, map);
        draw_UI_ITEM(LoadedTextures["images/textures/Numbers/0.png"], 1, 0, 1, 1, map);
        draw_UI_ITEM(LoadedTextures["images/textures/Numbers/" + std::string(1, WALLET_string[0]) + ".png"], 2, 0, 1, 1, map);
    }
    else if (WALLET_string.size() == 2)
    {
        draw_UI_ITEM(LoadedTextures["images/textures/Numbers/0.png"], 0, 0, 1, 1, map);
        draw_UI_ITEM(LoadedTextures["images/textures/Numbers/" + std::string(1, WALLET_string[0]) + ".png"], 1, 0, 1, 1, map);
        draw_UI_ITEM(LoadedTextures["images/textures/Numbers/" + std::string(1, WALLET_string[1]) + ".png"], 2, 0, 1, 1, map);
    }
    else
    {
        draw_UI_ITEM(LoadedTextures["images/textures/Numbers/" + std::string(1, WALLET_string[0]) + ".png"], 0, 0, 1, 1, map);
        draw_UI_ITEM(LoadedTextures["images/textures/Numbers/" + std::string(1, WALLET_string[1]) + ".png"], 1, 0, 1, 1, map);
        draw_UI_ITEM(LoadedTextures["images/textures/Numbers/" + std::string(1, WALLET_string[2]) + ".png"], 2, 0, 1, 1, map);
    }

    glPopMatrix();
    glPopMatrix();
}

void UI::show_WAVE(Map &map, size_t current_WAVE_id, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    std::string WAVE_string = std::to_string(current_WAVE_id);
    glPushMatrix();
    glTranslatef(1.4, -0.25, 0);
    draw_UI_ITEM(LoadedTextures["images/textures/Wave/Wave_Panel.png"], -2.5, 0, 3, 5, map);
    draw_UI_ITEM(LoadedTextures["images/textures/Wave/Wave_Indicator.png"], 0, 0, 4, 8, map);
    glPushMatrix();
    glTranslatef(0.05, 0.08, 0);
    for (float i{0}; i < WAVE_string.size(); i++)
    {
        std::string path = "images/textures/Numbers/" + std::string(1, WAVE_string[i]) + ".png";
        draw_UI_ITEM(LoadedTextures[path], i, 4, 2, 2, map);
    }
    glPopMatrix();
    glPopMatrix();
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
            draw_UI_ITEM(LoadedTextures["images/textures/Help/Help-Ennemies.png"], 18, 5, 5, 8, map);
    }
}

void UI::show_TOWER_PROPERTY(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures, std::unordered_map<int, Tower> &current_TOWERS_in_MAP)
{
    for (auto &tower : current_TOWERS_in_MAP)
    {
        if (tower.second.showProperty)
            draw_UI_ITEM(LoadedTextures["images/textures/Help/Help-Allies.png"], 18, 5, 5, 8, map);
    }
}

void UI::show_TOWER_to_select(Map &map, Tower const &tower, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    draw_UI_ITEM(tower.texture, tower.UI_pos.x, tower.UI_pos.y, tower.UI_size, tower.UI_size, map);
    std::string TOWER_price = std::to_string(tower.price);
    glPushMatrix();
    glTranslatef(-0.035, -0.03, 0);
    draw_UI_ITEM(LoadedTextures["images/textures/Other/Money_02.png"], tower.UI_pos.x, tower.UI_pos.y - 1, tower.UI_size - 1, tower.UI_size - 1, map);
    for (size_t i{0}; i < TOWER_price.size(); i++)
        draw_UI_ITEM(LoadedTextures["images/textures/Numbers/" + std::string(1, TOWER_price[i]) + ".png"], tower.UI_pos.x + i + 1, tower.UI_pos.y - 1, tower.UI_size - 1, tower.UI_size - 1, map);
    glPopMatrix();
}

void UI::show_PAUSE(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    draw_UI_ITEM(LoadedTextures["images/textures/Other/Pause_Pannel.png"], 4, 6, 7, 3, map);
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

// BONUS
void UI::show_TEAM(Map &map, std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    draw_UI_ITEM(LoadedTextures["images/textures/Team/Team_Panel.png"], 4, 14, 7, 3, map);
    draw_UI_ITEM(LoadedTextures["images/textures/Team/poke_Leo.jpg"], -8.5, 1, 8, 11, map);
    draw_UI_ITEM(LoadedTextures["images/textures/Team/poke_Max.jpg"], 3.5, 1, 8, 11, map);
    draw_UI_ITEM(LoadedTextures["images/textures/Team/poke_Anass_02.png"], 15.5, 1, 8, 11, map);
}