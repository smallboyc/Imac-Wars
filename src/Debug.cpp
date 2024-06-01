#include "Game.hpp"

// Informations de tous les pixels dans PIXELS
void Map::display_PIXELS_informations()
{
    for (Pixel pixel : this->PIXELS)
    {
        std::cout << "(" << pixel.x << "," << pixel.y << ") -> (" << pixel.color.r << "," << pixel.color.g << "," << pixel.color.b << ")";
        if (pixel.is_NODE)
            std::cout << " = NODE";
        if (pixel.is_START_POINT)
            std::cout << " + START ";
        if (pixel.is_END_POINT)
            std::cout << " + END ";
        if (pixel.is_PATH)
            std::cout << " -> PATH ";
        if (pixel.is_VOID)
            std::cout << " -> VOID ";

        Connections &NEIGHBOUR{pixel.PIXEL_connection};
        std::cout << " & connected to => ";
        if (NEIGHBOUR.top != nullptr)
            std::cout << "TOP : " << " (" << NEIGHBOUR.top->x << ":" << NEIGHBOUR.top->y << ") ";
        else
            std::cout << "TOP : " << " ? ";
        if (NEIGHBOUR.bottom != nullptr)
            std::cout << "BOTTOM : " << " (" << NEIGHBOUR.bottom->x << ":" << NEIGHBOUR.bottom->y << ") ";
        else
            std::cout << "BOTTOM : " << " ? ";
        if (NEIGHBOUR.left != nullptr)
            std::cout << "LEFT : " << " (" << NEIGHBOUR.left->x << ":" << NEIGHBOUR.left->y << ") ";
        else
            std::cout << "LEFT : " << " ? ";
        if (NEIGHBOUR.right != nullptr)
            std::cout << "RIGHT : " << " (" << NEIGHBOUR.right->x << ":" << NEIGHBOUR.right->y << ") ";
        else
            std::cout << "RIGHT : " << " ? ";

        std::cout << std::endl;
    }
}

// Affiche tous les plus courts chemins
void Map::display_SHORTER_PATH()
{
    for (auto &shorter_path : this->SHORTER_PATH_LIST)
        for (Node &node : shorter_path)
            std::cout << node.id << " : (" << node.pixel.x << "," << node.pixel.y << ")" << std::endl;
}