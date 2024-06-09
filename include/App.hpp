#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include <vector>

#include "Game.hpp"

class App
{
public:
    App();

    void setup();
    void update();

    // GLFW callbacks binding
    void key_callback(int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow *window,int button, int action, int mods);
    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    void size_callback(GLFWwindow *window, int width, int height);

private:
    void render();

    int _width{};
    int _height{};
    double _previousTime{};
    float _viewSize{};
    TowerDefense TD;
};