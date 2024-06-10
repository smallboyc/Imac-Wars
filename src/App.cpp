#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <filesystem>

#include <sstream>

#include "App.hpp"
#include "Tower.hpp"

App::App() : _previousTime(0.0), _viewSize(1.5)
{
    Game::LOAD(TD, "map_schema_15x15_02.itd", 15);
}

void App::setup()
{
    Game::SETUP(TD);
}

void App::update()
{
    const double currentTime{glfwGetTime()};
    const double elapsedTime{currentTime - _previousTime};
    _previousTime = currentTime;

    Game::UPDATE(TD, elapsedTime, currentTime);

    render();
}

void App::render()
{
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    Game::RENDER(TD, _width, _height);
}

void App::key_callback(int key, int scancode, int action, int mods)
{
    Game::active_KEY_CALLBACK(TD, key, scancode, action, mods);
}

void App::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
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
            if (hover_TOWER_in_UI({mouseX, mouseY}, tower.second.UI_pos, tower.second.UI_size) && tower.second.can_be_Selected)
            {
                TD.current_TOWER_id = tower.second.type;
            }
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

void App::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
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
        if (hover_TOWER_in_UI({mouseX, mouseY}, tower.second.UI_pos, tower.second.UI_size))
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

void App::size_callback(GLFWwindow *window, int width, int height)
{
    //(écran rétina) On associe les dimensions de la fenêtre au FrameBuffer
    _width = width;
    _height = height;

    glfwGetFramebufferSize(window, &_width, &_height);
    glViewport(0, 0, _width, _height);

    const float aspectRatio{_width / static_cast<float>(_height)};

    // Change the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspectRatio > 1.0f)
    {
        glOrtho(-_viewSize / 2.0f * aspectRatio, _viewSize / 2.0f * aspectRatio, -_viewSize / 2.0f, _viewSize / 2.0f, -1.0f, 1.0f);
    }
    else
    {
        glOrtho(-_viewSize / 2.0f, _viewSize / 2.0f, -_viewSize / 2.0f / aspectRatio, _viewSize / 2.0f / aspectRatio, -1.0f, 1.0f);
    }
}
