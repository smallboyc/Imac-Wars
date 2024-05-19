#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include <sstream>
#include <fstream>
#include <cmath>

#include "simpletext.h"
#include "Map.hpp"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include "UI.hpp"

void UI::enabled(Map &map)
{
    if (this->SHOW_TARGETED_CELL)
        draw_quad(this->x, this->y, map);
}