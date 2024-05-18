#include "utils.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include "Map.hpp"

std::filesystem::path make_absolute_path(std::filesystem::path const &path, bool check_path_exists)
{
    auto const res{path.is_relative() ? CMAKE_SOURCE_DIR / path : path};

    if (check_path_exists && !std::filesystem::exists(res))
    {
        using namespace std::literals;
        auto const msg{"Path \""s + res.string() + "\" does not exist. Make sure you didn't misspell it or made it relative to the wrong folder. All relative paths will be relative to the directory containing your CMakeLists.txt file."};
        std::cerr << msg << '\n';
        throw std::runtime_error{msg};
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

void set_IN_OUT_orientation_texture(Connections const &NEIGHBOUR, std::vector<std::filesystem::path> &TILE_path_list)
{
    if (!NEIGHBOUR.top->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0112.png");
    else if (!NEIGHBOUR.bottom->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0100.png");
    else if (!NEIGHBOUR.left->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0113.png");
    else
        TILE_path_list.push_back("images/Tiles/tile_0101.png");
}

void set_NODE_orientation_texture(Connections const &NEIGHBOUR, std::vector<std::filesystem::path> &TILE_path_list)
{
    if (NEIGHBOUR.top->is_VOID && NEIGHBOUR.right->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0075.png");
    else if (NEIGHBOUR.top->is_VOID && NEIGHBOUR.left->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0073.png");
    else if (NEIGHBOUR.bottom->is_VOID && NEIGHBOUR.right->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0099.png");
    else if (NEIGHBOUR.bottom->is_VOID && NEIGHBOUR.left->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0097.png");
    else if (NEIGHBOUR.top->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0088.png");
    else if (NEIGHBOUR.bottom->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0089.png");
    else if (NEIGHBOUR.left->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0076.png");
    else
        TILE_path_list.push_back("images/Tiles/tile_0077.png");
}

void set_PATH_orientation_texture(Connections const &NEIGHBOUR, std::vector<std::filesystem::path> &TILE_path_list)
{
    if (NEIGHBOUR.top->is_VOID && NEIGHBOUR.bottom->is_VOID)
        TILE_path_list.push_back("images/Tiles/tile_0098.png");
    else
        TILE_path_list.push_back("images/Tiles/tile_0087.png");
}

// Dessin un sprite à la position (x,y) sur une map
void draw_quad_with_texture(GLuint const &texture, float &x, float &y, Map &map)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f((-map.SEMI_MAP_SIZE + x) / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE, (-map.SEMI_MAP_SIZE + y) / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE);

    glTexCoord2d(1, 0);
    glVertex2f((map.SEMI_MAP_SIZE + x) / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE, (-map.SEMI_MAP_SIZE + y) / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE);

    glTexCoord2d(1, 1);
    glVertex2f((map.SEMI_MAP_SIZE + x) / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE, (map.SEMI_MAP_SIZE + y) / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE);

    glTexCoord2d(0, 1);
    glVertex2f((-map.SEMI_MAP_SIZE + x) / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE, (map.SEMI_MAP_SIZE + y) / map.NUMBER_OF_PIXELS_IN_LINE - map.SEMI_MAP_SIZE);

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void drawOrigin()
{
    glPointSize(10);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2d(0, 0);
    glVertex2d(1, 0);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2d(0, 0);
    glVertex2d(0, 1);
    glEnd();
}