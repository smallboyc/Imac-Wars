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
// struct Connections
// {
//     Tile *top;
//     Tile *bottom;
//     Tile *right;
//     Tile *left;
// };
struct Pixel;
struct Connections
{
    Pixel *top;
    Pixel *bottom;
    Pixel *right;
    Pixel *left;
};

struct Pixel
{
    int x{0};
    int y{0};
    Color color;
    bool is_NODE{false};
    bool is_VOID{false};
    bool is_BUSY{false};
    Connections PIXEL_connection;
};

// struct Tile;

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
    // bool is_NODE{false};
    // bool is_VOID{false};
    // bool is_BUSY{false};
    // Connections TILE_connection;
};

std::filesystem::path
make_absolute_path(std::filesystem::path const &path, bool check_path_exists = true);

template <glm::length_t N, typename T, glm::qualifier Q>
std::ostream &operator<<(std::ostream &os, glm::vec<N, T, Q> const &vec);

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &vec);

bool operator==(Color const &color_1, Color const &color_2);
bool operator==(Pixel const &pixel_1, Pixel const &pixel_2);

Color get_colors_from_itd(std::string const &type);

// ça c'est pour debug en cout (:

// for (Node node : map.NODES)
// {
//     std::cout << node.id << " : (" << node.x << "," << node.y << ") -> (" << node.color.r << "," << node.color.g << "," << node.color.b << ") " << std::endl;
// }