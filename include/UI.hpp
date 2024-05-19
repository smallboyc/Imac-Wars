#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <img/img.hpp>
#include <glad/glad.h>
#include <simpletext.h>
#include "utils.hpp"
#include "Map.hpp"

struct UI
{
    float x{0};
    float y{0};
    bool SHOW_TARGETED_CELL{false};
    void enabled(Map &map);
};