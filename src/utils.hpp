#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Color
{
    int r;
    int g;
    int b;
};
struct Pixel
{
    int x{0};
    int y{0};
    Color color;
};

struct Tile
{
    Pixel pixel;
    GLuint texture;
    std::filesystem::path path;
};

std::filesystem::path
make_absolute_path(std::filesystem::path const &path, bool check_path_exists = true);

template <glm::length_t N, typename T, glm::qualifier Q>
std::ostream &operator<<(std::ostream &os, glm::vec<N, T, Q> const &vec);

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &vec);

bool operator==(Color color_1, Color color_2);

Color get_colors_from_itd(std::string const &type);