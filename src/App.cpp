#include "App.hpp"

App::App() : _previousTime(0.0), _viewSize(1.5)
{
    Game::LOAD(TD);
}

void App::setup()
{
    Game::SETUP(TD, "map_schema_15x15_02.itd", 15);
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
    Game::active_MOUSE_CLICK_CALLBACK(TD, window, _viewSize, button, action, mods);
}

void App::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    Game::active_MOUSE_POSITION_CALLBACK(TD, window, _viewSize, xpos, ypos);
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
