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
#include "App.hpp"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include "Wave.hpp"

void Wave::set_difficulty_from_ITD(int &level)
{
    this->level = level;
}
