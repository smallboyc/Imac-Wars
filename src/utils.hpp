#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Color
{
    int r{0};
    int g{0};
    int b{0};
};

struct Pixel
{
    int x{0};
    int y{0};
    Color color;
};

struct Node
{
    int id{0};
    Pixel pixel;
    int connected_to{0};
};

struct Tile
{
    Pixel pixel;
    GLuint texture;
    std::filesystem::path path;
    bool is_NODE{false};
};

std::filesystem::path
make_absolute_path(std::filesystem::path const &path, bool check_path_exists = true);

template <glm::length_t N, typename T, glm::qualifier Q>
std::ostream &operator<<(std::ostream &os, glm::vec<N, T, Q> const &vec);

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &vec);

bool operator==(Color color_1, Color color_2);
bool operator==(Pixel pixel_1, Pixel pixel_2);

Color get_colors_from_itd(std::string const &type);

// ça c'est pour debug en cout (:

//   for (Tile tile : map.TILES)
//     {
//         std::cout << "(" << tile.pixel.x << "," << tile.pixel.y << ") -> (" << tile.pixel.color.r << "," << tile.pixel.color.g << "," << tile.pixel.color.b << ")";
//         if (tile.is_NODE)
//             std::cout << " = NODE";
//         std::cout << std::endl;
//     }

// for (Node node : map.NODES)
// {
//     std::cout << node.id << " : (" << node.x << "," << node.y << ") -> (" << node.color.r << "," << node.color.g << "," << node.color.b << ") " << std::endl;
// }