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
    // Type de pixel
    bool is_NODE{false};
    bool is_VOID{false};
    bool is_PATH{false};
    bool is_START_POINT{false};
    bool is_END_POINT{false};
    Connections PIXEL_connection;
};

struct Node
{
    int id{0};
    Pixel pixel;
    int connected_to{0};
};

// struct Node
// {
//     int id{0};
//     Pixel pixel;
//     Node *connected_to;
// };

struct Tile
{
    Pixel pixel;
    std::vector<GLuint> texture_list;
    std::vector<std::filesystem::path> path_list;
};

std::filesystem::path
make_absolute_path(std::filesystem::path const &path, bool check_path_exists = true);

template <glm::length_t N, typename T, glm::qualifier Q>
std::ostream &operator<<(std::ostream &os, glm::vec<N, T, Q> const &vec);

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &vec);

bool operator==(Color const &color_1, Color const &color_2);
bool operator==(Pixel const &pixel_1, Pixel const &pixel_2);

void set_IN_OUT_orientation_texture(Connections const &NEIGHBOUR, std::vector<std::filesystem::path> &TILE_path_list);
void set_NODE_orientation_texture(Connections const &NEIGHBOUR, std::vector<std::filesystem::path> &TILE_path_list);
void set_PATH_orientation_texture(Connections const &NEIGHBOUR, std::vector<std::filesystem::path> &TILE_path_list);

// ça c'est pour debug en cout (:

// for (Node node : map.NODES)
//     {
//         std::cout << node.id << " : (" << node.pixel.x << "," << node.pixel.y << ")" << std::endl;
//     }

// for (auto pixel : map.PIXELS)
// {
//     std::cout << "(" << pixel.x << ":" << pixel.y << ")" << " = " << "{" << pixel.color.r << "," << pixel.color.g << "," << pixel.color.b << "}" << std::endl;
// }
