#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include "Map.hpp"
#include "utils.hpp"
#include "Enemy.hpp"

std::filesystem::path make_absolute_path(std::filesystem::path const &path, bool check_path_exists)
{
    auto const res{path.is_relative() ? CMAKE_SOURCE_DIR / path : path};

    if (check_path_exists && !std::filesystem::exists(res))
    {
        using namespace std::literals;
        auto const msg{"ERREUR : Le chemin \""s + path.string() + "\" n'existe pas !"};
        std::cerr << msg << '\n';
        exit(EXIT_FAILURE);
    }

    return res;
}

template <glm::length_t N, typename T, glm::qualifier Q>
std::ostream &operator<<(std::ostream &os, glm::vec<N, T, Q> const &vec)
{
    os << "(";

    os << vec[0];
    for (size_t i{1}; i < N; ++i)
    {
        os << ", " << vec[i];
    }
    os << ")";
    return os;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &vec)
{
    if (vec.empty())
    {
        return os << "[]";
    }

    os << '[';
    for (size_t i{0}; i < vec.size() - 1; ++i)
    {
        os << vec[i] << ", ";
    }
    return os << vec.back() << ']';
}

bool operator==(Color const &color_1, Color const &color_2)
{
    if (color_1.r == color_2.r && color_1.g == color_2.g && color_1.b == color_2.b)
        return true;
    return false;
}

bool operator==(Pixel const &pixel_1, Pixel const &pixel_2)
{
    if (pixel_1.x == pixel_2.x && pixel_1.y == pixel_2.y)
        return true;
    return false;
}

std::ostream &operator<<(std::ostream &os, const Graph::WeightedGraph graph)
{
    for (std::pair p : graph.adjacency_list)
        for (Graph::WeightedGraphEdge edge : p.second)
            os << p.first << " -> " << edge.to << " ( " << edge.weight << " )" << std::endl;
    return os;
}

bool is_NODE(const Connections &NEIGHBOUR)
{
    bool top = NEIGHBOUR.top->is_PATH;
    bool bottom = NEIGHBOUR.bottom->is_PATH;
    bool left = NEIGHBOUR.left->is_PATH;
    bool right = NEIGHBOUR.right->is_PATH;

    int pathCount = top + bottom + left + right;

    // CROSS
    if (pathCount == 4)
        return true;

    // ANGLE
    if (pathCount == 2)
    {
        if ((top && right) || (top && left) || (bottom && left) || (bottom && right))
            return true;
    }

    // INTERSECTION
    if (pathCount == 3)
        return true;

    // ALONE
    if (pathCount == 1)
        return true;

    return false;
}
void set_IN_OUT_orientation_texture(Connections const &NEIGHBOUR, std::vector<std::filesystem::path> &TILE_path_list)
{
    bool top = NEIGHBOUR.top->is_PATH;
    bool bottom = NEIGHBOUR.bottom->is_PATH;
    bool left = NEIGHBOUR.left->is_PATH;

    if (top)
        TILE_path_list.push_back("images/textures/Map/Tip_Bottom.png");
    else if (bottom)
        TILE_path_list.push_back("images/textures/Map/Tip_Top.png");
    else if (left)
        TILE_path_list.push_back("images/textures/Map/Tip_Right.png");
    else
        TILE_path_list.push_back("images/textures/Map/Tip_Left.png");
}

void set_NODE_orientation_texture(Connections const &NEIGHBOUR, std::vector<std::filesystem::path> &TILE_path_list)
{

    bool top = NEIGHBOUR.top->is_PATH;
    bool bottom = NEIGHBOUR.bottom->is_PATH;
    bool left = NEIGHBOUR.left->is_PATH;
    bool right = NEIGHBOUR.right->is_PATH;

    if (!top && !right)
        TILE_path_list.push_back("images/textures/Map/Angle_LB.png");
    else if (!top && !left)
        TILE_path_list.push_back("images/textures/Map/Angle_RB.png");
    else if (!bottom && !right)
        TILE_path_list.push_back("images/textures/Map/Angle_LT.png");
    else if (!bottom && !left)
        TILE_path_list.push_back("images/textures/Map/Angle_RT.png");
    else if (!top)
        TILE_path_list.push_back("images/textures/Map/Inter_Bottom.png");
    else if (!bottom)
        TILE_path_list.push_back("images/textures/Map/Inter_Top.png");
    else if (!left)
        TILE_path_list.push_back("images/textures/Map/Inter_Right.png");
    else
        TILE_path_list.push_back("images/textures/Map/Inter_Left.png");
}

void set_PATH_orientation_texture(Connections const &NEIGHBOUR, std::vector<std::filesystem::path> &TILE_path_list)
{
    bool top = NEIGHBOUR.top->is_PATH;
    bool bottom = NEIGHBOUR.bottom->is_PATH;

    if (!top && !bottom)
        TILE_path_list.push_back("images/textures/Map/Path_Horizontal.png");
    else
        TILE_path_list.push_back("images/textures/Map/Path_Vertical.png");
}

// Si l'ennemi est sur un chemin => OK sinon PAS OK.
bool is_ENEMY_travel_correctly(Enemy &enemy, Map &map)
{
    for (Pixel &pixel : map.PIXELS)
        if (std::round(enemy.pos.x) == pixel.x && std::round(enemy.pos.y) == pixel.y)
        {
            if (pixel.is_PATH)
                return true;
            else
                return false;
        }
}

bool is_inside_MAP(float &x, float &y, Map &map)
{
    return (x >= 0 && x < map.NUMBER_OF_PIXELS_IN_LINE - 1 && y >= 0 && y < map.NUMBER_OF_PIXELS_IN_LINE - 1);
}

bool hover_ELEMENT_in_UI(glm::vec2 const &mouse_cursor, glm::vec2 const &element_pos, int const &UI_size)
{
    return (mouse_cursor.x >= std::round(element_pos.x) && mouse_cursor.x <= std::round(element_pos.x) + UI_size &&
            mouse_cursor.y >= std::round(element_pos.y) && mouse_cursor.y <= std::round(element_pos.y) + UI_size);
}