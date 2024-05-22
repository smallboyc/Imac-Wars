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

App::App() : _previousTime(0.0), _viewSize(2.0)
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
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);
}

void App::update()
{

    const double currentTime{glfwGetTime()};
    const double elapsedTime{currentTime - _previousTime};
    _previousTime = currentTime;

    TD.update_WAVE();
    TD.update_ENEMIES(elapsedTime);

    _angle += 10.0f * elapsedTime;
    _angle = std::fmod(_angle, 360.0f);

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

    TD.render_MAP();
    TD.render_ENEMIES();
    TD.active_UI();

    // Text zone
    // TextRenderer.Label("- IMAC TOWER DEFENSE - ", _width, 20, SimpleText::CENTER);

    // // Without set precision
    // const std::string angle_label_text{"Angle: " + std::to_string(_angle)};

    // // Using stringstream to format the string with fixed precision
    // std::stringstream stream{};
    // stream << std::fixed << "Angle: " << std::setprecision(2) << _angle;
    // stream << std::fixed << "MAP : " << map.NUMBER_OF_PIXELS_IN_LINE << " X " << map.NUMBER_OF_PIXELS_IN_LINE << " PIXELS";
    // // angle_label_text = stream.str();

    // TextRenderer.Label(angle_label_text.c_str(), _width / 2, _height - 4, SimpleText::CENTER);

    // TextRenderer.Render();
}

void App::key_callback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        TD.ui.SHOW_TARGETED_CELL = !TD.ui.SHOW_TARGETED_CELL;
    }
    // if (key == GLFW_KEY_W && action == GLFW_PRESS)
    // {
    //     michel.health -= 0.01;
    // }

    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && TD.ui.SHOW_TARGETED_CELL)
    {
        float top_neighbour{TD.ui.y + 1};
        float bottom_neighbour{TD.ui.y - 1};
        float right_neighbour{TD.ui.x + 1};
        float left_neighbour{TD.ui.x - 1};
        if (key == GLFW_KEY_UP && is_inside_MAP(TD.ui.x, top_neighbour, TD.map))
            TD.ui.y++;
        if (key == GLFW_KEY_DOWN && is_inside_MAP(TD.ui.x, bottom_neighbour, TD.map))
            TD.ui.y--;
        if (key == GLFW_KEY_LEFT && is_inside_MAP(left_neighbour, TD.ui.y, TD.map))
            TD.ui.x--;
        if (key == GLFW_KEY_RIGHT && is_inside_MAP(right_neighbour, TD.ui.y, TD.map))
            TD.ui.x++;
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
}

void App::size_callback(int width, int height)
{
    _width = width;
    _height = height;
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, _width, _height);

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
