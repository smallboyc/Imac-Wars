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

#include "Game.hpp"

ma_sound mainThemeSound;
ma_sound imperialMarchSound;
ma_sound cantinaSound;
ma_sound throneRoomSound;
ma_sound bonusSound;
ma_sound clickSound;

std::vector<std::pair<std::string, ma_sound *>> sounds = {
    {"../../sound/Main_Theme.mp3", &mainThemeSound},
    {"../../sound/Imperial_March.mp3", &imperialMarchSound},
    {"../../sound/Cantina.mp3", &cantinaSound},
    {"../../sound/Throne_Room.mp3", &throneRoomSound},
    {"../../sound/Bonus.mp3", &bonusSound},
    {"../../sound/Click.mp3", &clickSound}};

void Game::LOAD(TowerDefense &TD)
{
    // Textures
    TD.Load_All_Textures();

    // Sons
    ma_engine_set_volume(&SoundEngine::GetEngine(), 0.5f);
    TD.Load_All_Sounds(sounds);

    // On joue les sons en boucle
    for (auto &sound : sounds)
        ma_sound_set_looping(sound.second, MA_TRUE);
}

void Game::SETUP(TowerDefense &TD, std::string const &MAP_SCHEMA_ITD_path, int const &pixel_UNIT)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black window

    TD.setup_MAP(MAP_SCHEMA_ITD_path, pixel_UNIT);
    TD.get_TOWERS_from_ITD();
    TD.get_ENEMIES_from_ITD();
    TD.get_WAVES_from_ITD();
    TD.get_SPRITE_SHEETS_from_ITD();
    TD.setup_WAVE();
    TD.get_ENEMIES_into_WAVE();
    TD.setup_ENEMIES_in_WAVE();
    TD.setup_SPRITE_SHEETS();
    TD.setup_BASE();
    TD.ui.setup_UI_Text();

    // STAR WARS Main title (sound)
    ma_sound_start(&mainThemeSound);
}
void Game::UPDATE(TowerDefense &TD, const double &elapsedTime, const double &currentTime)
{
    // Pour l'animation de IMAC WARS => récupère le temps
    TD.ui.get_TIME_in_UI(elapsedTime, currentTime);

    if (TD.GAME_IS_PLAYING && !TD.PAUSE && !TD.GAME_OVER)
    {
        TD.update_ENEMIES_in_WAVE(elapsedTime, currentTime);
        TD.update_TOWERS(elapsedTime, currentTime);
    }
}

void Game::RENDER(TowerDefense &TD, int &_width, int &_height)
{
    draw_WINDOW_Background(TD.LoadedTextures["images/textures/Other/window_background.png"]);

    if (TD.GAME_OVER) // SI LE JOUEUR PERD => Active l'écran de GAME OVER
    {
        ma_sound_stop(&imperialMarchSound);
        ma_sound_start(&throneRoomSound);
        TD.GAME_IS_PLAYING = false;
        TD.ui.show_GAME_OVER(_width, _height, TD.current_TOWERS_in_MAP);
        TD.ui.show_QUIT_GAME(_width, _height);
    }

    if (TD.PLAYER_WIN) // SI LE JOUEUR GAGNE => Active l'écran de WIN !
    {
        ma_sound_stop(&imperialMarchSound);
        ma_sound_start(&cantinaSound);
        TD.GAME_IS_PLAYING = false;
        TD.ui.show_PLAYER_WIN(_width, _height);
        TD.ui.show_QUIT_GAME(_width, _height);
    }

    if (TD.BONUS) // Le petit bonus qui fait plaiz
    {
        ma_sound_stop(&cantinaSound);
        ma_sound_start(&bonusSound);
        TD.ui.show_TEAM(TD.map, TD.LoadedTextures);
        TD.ui.show_QUIT_GAME(_width, _height);
    }
    
    if (TD.GAME_IS_PLAYING)
    {
        TD.render_MAP();

        if (!TD.PAUSE)
        {
            if (!TD.FINISHED_WAVE)
            {
                TD.ui.PLAY_PAUSE.Label("PRESS -SPACE- TO PAUSE", _width / 2, 150, SimpleText::CENTER);
                TD.render_ENEMIES_in_WAVE();
                TD.render_TOWERS();
                TD.render_BASE_health();
                TD.active_UI(_width, _height);
            }
        }
        else // JEU EN PAUSE
        {
            draw_BREAK_MENU(TD.map);
            TD.ui.show_PAUSE(TD.map, TD.LoadedTextures);
            TD.ui.PLAY_PAUSE.Label("PRESS -SPACE- TO PLAY", _width / 2, 150, SimpleText::CENTER);
            TD.ui.show_HELP_in_PAUSE(TD.map, TD.LoadedTextures);
            TD.ui.show_QUIT_GAME(_width, _height);
        }

        if (TD.FINISHED_WAVE)
        {
            draw_BREAK_MENU(TD.map);
            TD.ui.show_WAVE_FINISHED(_width, _height, TD.current_WAVE_id);
        }
    }
    else if (!TD.GAME_IS_PLAYING && !TD.GAME_OVER && !TD.PLAYER_WIN && !TD.BONUS) // MENU START
    {
        TD.ui.show_IMAC_WARS_TITLE(TD.map, TD.LoadedTextures);
        TD.ui.PLAY_PAUSE.Label("PRESS > S < TO START", _width / 2, _height - 100, SimpleText::CENTER);
    }
    TD.ui.PLAY_PAUSE.Render();
}

void Game::active_KEY_CALLBACK(TowerDefense &TD, int key, int scancode, int action, int mods)
{
    // Lancer le jeu
    if (!TD.GAME_OVER && !TD.PLAYER_WIN)
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            TD.GAME_IS_PLAYING = true;
            ma_sound_stop(&mainThemeSound);
            ma_sound_start(&imperialMarchSound);
        }

    // Si le jeu est en PAUSE, ou que c'est une fin de partie => possibilité de quitter le jeu.
    if (TD.PAUSE || TD.GAME_OVER || TD.PLAYER_WIN || TD.BONUS)
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
            exit(EXIT_SUCCESS);

    // Si le jeu est lancé
    if (TD.GAME_IS_PLAYING)
    {
        // Pause
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !TD.FINISHED_WAVE)
            TD.PAUSE = !TD.PAUSE;

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            for (auto &enemy : TD.current_ENEMIES_in_WAVE)
                enemy.second.showProperty = false;

            for (auto &tower : TD.current_TOWERS_in_MAP)
                tower.second.showProperty = false;
        }

        if (TD.FINISHED_WAVE)
        {
            if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
            {
                TD.ENEMIES_id_to_launch = 0;
                TD.current_WAVE_id++;
                TD.FINISHED_WAVE = false;
            }
        }
    }

    if (TD.PLAYER_WIN)
        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        {
            TD.BONUS = true;
            TD.PLAYER_WIN = false;
        }
}

void Game::active_MOUSE_CLICK_CALLBACK(TowerDefense &TD, GLFWwindow *window, float &_viewSize, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        float aspectRatio = windowWidth / static_cast<float>(windowHeight);

        double normalizedX = (2.0 * xpos / windowWidth - 1.0) * aspectRatio;
        double normalizedY = 1.0 - 2.0 * ypos / windowHeight;

        int mouseX = (normalizedX * 0.5 * _viewSize + TD.map.SEMI_MAP_SIZE) * TD.map.NUMBER_OF_PIXELS_IN_LINE;
        int mouseY = (normalizedY * 0.5 * _viewSize + TD.map.SEMI_MAP_SIZE) * TD.map.NUMBER_OF_PIXELS_IN_LINE;

        // Si le joueur click sur une tour dans l'UI, la tour est sélectionnée !
        for (auto &tower : TD.TOWERS_ITD)
        {
            if (hover_ELEMENT_in_UI({mouseX, mouseY}, tower.second.UI_pos, tower.second.UI_size) && tower.second.can_be_Selected)
            {
                TD.current_TOWER_id = tower.second.type;
            }
        }

        // Si un joueur click sur un ennemi, il peut voir ses propriétés
        for (auto &enemy : TD.current_ENEMIES_in_WAVE)
        {
            if (hover_ELEMENT_in_UI({mouseX, mouseY}, enemy.second.pos, 1)){
                enemy.second.showProperty = true;
                ma_sound_start(&clickSound);}
            else
                enemy.second.showProperty = false;
        }

        for (auto &tower : TD.current_TOWERS_in_MAP)
        {
            if (hover_ELEMENT_in_UI({mouseX, mouseY}, tower.second.pos, 1)){
                tower.second.showProperty = true;
                ma_sound_start(&clickSound);}
            else
                tower.second.showProperty = false;
        }

        // le joueur positionne une tour sur la map au click
        for (Pixel &pixel : TD.map.PIXELS)
        {
            // Si le joueur click sur un pixel de map.
            if (pixel.x == mouseX && pixel.y == mouseY && pixel.is_VOID && !pixel.is_FORBIDDEN && !pixel.is_TOWER)
            {
                // Le joueur peut poser sa tour s'il dispose de l'argent nécessaire.
                if (TD.ui.WALLET >= TD.TOWERS_ITD.at(TD.current_TOWER_id).price)
                {
                    Tower tower = TD.TOWERS_ITD.at(TD.current_TOWER_id);
                    tower.setup(TD.SPRITE_SHEETS_ITD, {pixel.x, pixel.y});
                    TD.current_TOWERS_in_MAP.insert({TD.towerID, tower});

                    TD.towerID++;
                    TD.ui.WALLET -= tower.price;
                    pixel.is_VOID = false;
                    pixel.is_TOWER = true;
                }
            }
        }
    }
}

void Game::active_MOUSE_POSITION_CALLBACK(TowerDefense &TD, GLFWwindow *window, float &_viewSize, double xpos, double ypos)
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Calculer l'aspect ratio pour x
    float aspectRatio = windowWidth / static_cast<float>(windowHeight);

    // Coordonnées normalisées [-1, 1]
    double normalizedX = (2.0 * xpos / windowWidth - 1.0) * aspectRatio;
    double normalizedY = 1.0 - 2.0 * ypos / windowHeight;

    int mouseX = (normalizedX * 0.5 * _viewSize + TD.map.SEMI_MAP_SIZE) * TD.map.NUMBER_OF_PIXELS_IN_LINE;
    int mouseY = (normalizedY * 0.5 * _viewSize + TD.map.SEMI_MAP_SIZE) * TD.map.NUMBER_OF_PIXELS_IN_LINE;

    // Affiche le curseur de sélection si on est sur l'item de tour.
    for (auto &tower : TD.TOWERS_ITD)
        if (hover_ELEMENT_in_UI({mouseX, mouseY}, tower.second.UI_pos, tower.second.UI_size))
            tower.second.hover = true;
        else
            tower.second.hover = false;

    // Affiche le curseur de positionnement si on est sur une case de la map.
    for (Pixel &pixel : TD.map.PIXELS)
    {
        if (pixel.x == mouseX && pixel.y == mouseY)
            pixel.on_Mouse_Over = true;
        else
            pixel.on_Mouse_Over = false;
    }
}