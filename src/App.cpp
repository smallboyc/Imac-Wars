#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <sstream>

#include "App.hpp"
#include "Map.hpp"
#include "Graph.hpp"
#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include "Enemy.hpp"
#include "UI.hpp"

Enemy michel;
UI interface;

App::App() : _previousTime(0.0), _viewSize(2.0)
{
    // MAP
    map.NUMBER_OF_PIXELS_IN_LINE = 10;
    map.schema_file = "map_schema_10x10_V4";
    map.get_NODES_from_ITD();
    map.create_GRAPH_from_NODES();
    map.get_SHORTER_PATH_from_dijkstra();
    map.generate_SCHEMA();
    map.get_PIXELS_from_SCHEMA();
    map.set_PIXELS_type();
    map.set_PIXELS_connected();
    map.get_TILES_from_PIXELS();
    map.render_TILES_texture();
    map.display_SHORTER_PATH();

    // ENEMY
    michel.set(map);
}

void App::setup()
{
    // Set the clear color to a nice blue
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

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

    i += michel.speed * elapsedTime;

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

    map.load_MAP();

    glPushMatrix();
    if (!michel.isDead)
        michel.action(map, i);
    glPopMatrix();

    if (interface.SHOW_TARGETED_CELL)
        draw_quad(interface.x, interface.y, map);

    // Text zone
    // TextRenderer.Label("- IMAC TOWER DEFENSE - ", _width / 2, 20, SimpleText::CENTER);

    // // Without set precision
    // const std::string angle_label_text { "Angle: " + std::to_string(_angle) };

    // // Using stringstream to format the string with fixed precision
    // std::string angle_label_text{};
    // std::stringstream stream{};
    // stream << std::fixed << "Angle: " << std::setprecision(2) << _angle;
    // stream << std::fixed << "MAP : " << map.NUMBER_OF_PIXELS_IN_LINE << " X " << map.NUMBER_OF_PIXELS_IN_LINE << " PIXELS";
    // // angle_label_text = stream.str();

    // TextRenderer.Label(angle_label_text.c_str(), _width / 2, _height - 4, SimpleText::CENTER);

    TextRenderer.Render();
}

void App::key_callback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        interface.SHOW_TARGETED_CELL = !interface.SHOW_TARGETED_CELL;
    }

    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && interface.SHOW_TARGETED_CELL)
    {
        if (key == GLFW_KEY_UP)
        {
            interface.y++;
        }
        else if (key == GLFW_KEY_DOWN)
        {
            interface.y--;
        }
        else if (key == GLFW_KEY_LEFT)
        {
            interface.x--;
        }
        else if (key == GLFW_KEY_RIGHT)
        {
            interface.x++;
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
