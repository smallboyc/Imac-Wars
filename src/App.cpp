#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <memory>
#include <sstream>

#include "App.hpp"
#include "Map.hpp"
#include "Graph.hpp"
#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include "Enemy.hpp"
#include "UI.hpp"

App::App() : _previousTime(0.0), _viewSize(1.5)
{
    TD.setup_MAP();
    TD.get_ENEMIES_from_ITD();
    TD.get_WAVES_from_ITD();
    TD.setup_WAVE();
    TD.get_ENEMIES_into_WAVE();
    TD.setup_ENEMIES();
}

void App::setup()
{
    // Set the clear color to a nice blue
    glClearColor(0.0f, 0.0f, 0.24f, 1.0f);

    // Setup the text renderer with blending enabled and white text color
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::CYAN);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.SetTextSize(SimpleText::FontSize::SIZE_32);
    TextRenderer.EnableBlending(true);
}

void App::update()
{
    const double currentTime{glfwGetTime()};
    const double elapsedTime{currentTime - _previousTime};
    _previousTime = currentTime;
    if (TD.GAME_IS_PLAYING && !TD.PAUSE) // Si le jeu est lancé et on est pas en pause
    {
        TD.update_WAVE();
        TD.update_ENEMIES(elapsedTime);
    }
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
    if (TD.GAME_IS_PLAYING)
    {
        TD.render_MAP();

        if (!TD.PAUSE)
        {
            TextRenderer.Label("PRESS -SPACE- TO PLAY", _width / 2, 150, SimpleText::CENTER);
            TD.render_ENEMIES();
            TD.active_UI();
        }
        else
        {
            TextRenderer.Label("> PAUSE <", _width / 2, _height / 2, SimpleText::CENTER);
            TextRenderer.Label("PRESS -SPACE- TO PAUSE", _width / 2, 150, SimpleText::CENTER);
            draw_BREAK_MENU(TD.map);
        }
    }
    else
    {
        TextRenderer.Label("PRESS > S < TO START", _width / 2, _height / 2, SimpleText::CENTER);
    }

    TextRenderer.Label("- IMAC TOWER DEFENSE -", _width / 2, 100, SimpleText::CENTER);
    TextRenderer.Render();
}

void App::key_callback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        TD.GAME_IS_PLAYING = true;
    if (TD.GAME_IS_PLAYING)
    {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            TD.PAUSE = !TD.PAUSE;

        if (!TD.PAUSE)
        {
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

void App::mouse_button_callback(int button, int action, int mods)
{
}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/)
{
}

void App::cursor_position_callback(double xpos, double ypos)
{
    // // Calcul des coordonnées X normalisées sur la plage [-1, 1]
    // double normalizedX = (2.0 * xpos / _width) - 1.0;

    // // Calcul des coordonnées Y normalisées sur la plage [-1, 1]
    // double normalizedY = (2.0 * ypos / _height) - 1.0;

    // // Affichage des coordonnées normalisées
    // std::cout << normalizedX << " : " << normalizedY << std::endl;
}

void App::size_callback(GLFWwindow *window, int width, int height)
{
    _width = width;
    _height = height;

    int veiwport_width{};
    int veiwport_height{};

    glfwGetFramebufferSize(window, &_width, &_height);
    glViewport(0, 0, veiwport_width, veiwport_height);

    const float aspectRatio{_width / (float)_height};

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
