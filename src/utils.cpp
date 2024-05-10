#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>

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

// Récupère la couleur RGB du Pixel : Path, Out, In dans ITD
Color get_colors_from_itd(std::string const &type)
{
    Color color;
    // Open the input file named "input.txt"
    std::ifstream inputFile("../../data/map.itd");
    std::vector<std::string> color_str_array{};
    std::string line;

    while (getline(inputFile, line))
    {
        // Récupération des paramètres des nodes.
        if (line.find(type) != std::string::npos)
        {
            std::string color_str{};

            for (size_t i{1}; i < line.size(); i++)
            {
                if (isdigit(line[i]) || line[i] == ' ')
                    color_str += line[i];
            }

            std::istringstream iss(color_str);
            std::vector<int> pixel_color;
            std::string token;

            while (std::getline(iss, token, ' '))
            {
                if (!token.empty())
                    pixel_color.push_back(std::stoi(token));
            }
            color = {pixel_color[0], pixel_color[1], pixel_color[2]};
        }
    }
    inputFile.close();

    return color;
}