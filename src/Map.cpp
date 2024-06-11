#include "Game.hpp"
#include "Map.hpp"
#include "Cell.hpp"

// Créer le GRAPH depuis le tableau de nodes
void Map::create_GRAPH_from_NODES()
{
    for (Node &node : this->NODES)
        for (int connected_node_id : node.connected_to)
            for (Node &node_to_connect : this->NODES)
                if (connected_node_id == node_to_connect.id)
                {
                    float distance = (std::abs(node.pixel.x - node_to_connect.pixel.x) + std::abs(node.pixel.y - node_to_connect.pixel.y));
                    this->GRAPH.add_directed_edge(node.id, node_to_connect.id, distance);
                }
}

// Détermine tous les plus courts chemins du GRAPH en les stockant dans un vector
void Map::get_SHORTER_PATH_LIST()
{
    std::vector<int> END_POINTS;    // Spawns ennemi (plusieurs)
    bool START_POINT_exists{false}; // Bases à défendre (unique)
    int START_POINT{0};             // id de la Base

    for (Node &node : this->NODES)
    {
        // On va chercher les END POINTS
        if (node.connected_to.empty())
            END_POINTS.push_back(node.id);
    }

    for (Pixel &pixel : this->PIXELS)
    {
        if (pixel.is_START_POINT)
            START_POINT_exists = true;
    }

    // On check s'il existe des END_POINTS. Sinon : erreur
    if (END_POINTS.empty())
    {
        std::cout << "ERREUR : Aucun spawn d'ennemi (End points) trouvé..." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    else if (!START_POINT_exists) // On check s'il existe un START POINT. Sinon : erreur
    {
        std::cout << "ERREUR : Aucune Base à défendre (Start point) trouvée..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Pour chaque END POINTS, on leur attribue un plus court chemin par rapport à l'entrée (START POINT)
    for (int end_point : END_POINTS)
    {
        std::unordered_map<int, std::pair<float, int>> DISTANCES{this->GRAPH.dijkstra(START_POINT, end_point)};
        std::vector<int> SHORTER_PATH_ID;
        std::vector<Node> SHORTER_PATH;
        auto finalEdge{DISTANCES.at(end_point)};
        // std::cout << "Distance minimale : " << finalEdge.first << std::endl;
        // std::cout << end << " -> ";
        SHORTER_PATH_ID.push_back(end_point);
        while (finalEdge.second != START_POINT)
        {
            SHORTER_PATH_ID.push_back(finalEdge.second);
            // std::cout << finalEdge.second << " -> ";
            finalEdge = DISTANCES.at(finalEdge.second);
        }
        SHORTER_PATH_ID.push_back(START_POINT);

        // Du tableau d'id, on en déduit les nodes composants le chemin le plus court dans l'ordre.
        for (int shorter_node_id : SHORTER_PATH_ID)
        {
            for (Node &node : this->NODES)
                if (node.id == shorter_node_id)
                    SHORTER_PATH.push_back(node);
        }
        this->SHORTER_PATH_LIST.push_back(SHORTER_PATH);
        // std::cout << start << std::endl;
    }
}

// Récupère tous les pixels du SCHEMA dans PIXELS
void Map::get_PIXELS_from_SCHEMA()
{
    std::vector<Pixel> PIXELS;

    Pixel pixel;

    for (size_t i = 0; i < this->SCHEMA.data_size(); i += this->SCHEMA.channels_count())
    {
        pixel.x = i / this->SCHEMA.channels_count() % this->NUMBER_OF_PIXELS_IN_LINE;
        pixel.y = i / this->SCHEMA.channels_count() / this->NUMBER_OF_PIXELS_IN_LINE;
        pixel.color = {(int)*(this->SCHEMA.data() + i), (int)*(this->SCHEMA.data() + i + 1), (int)*(this->SCHEMA.data() + i + 2)};
        PIXELS.push_back(pixel);
    }
    this->PIXELS = PIXELS;
}

// Donne un type à tous les pixels (chemin,herbe,noeud...) grâce au couleurs de l'ITD
void Map::set_PIXELS_type()
{
    for (Pixel &pixel : this->PIXELS)
    {
        if (pixel.color == get_colors_from_ITD("in"))
        {
            pixel.is_START_POINT = true;
            pixel.is_PATH = true;
        }
        else if (pixel.color == get_colors_from_ITD("out"))
        {
            pixel.is_END_POINT = true;
            pixel.is_PATH = true;
        }
        else if (pixel.color == get_colors_from_ITD("forbidden"))
        {
            pixel.is_FORBIDDEN = true;
            pixel.is_VOID = true;
        }
        else if (pixel.color == get_colors_from_ITD("path"))
            pixel.is_PATH = true;
        else
            pixel.is_VOID = true;
    }
}

// Connecte tous les pixels entre eux => Connaissance forte de l'entourage
void Map::set_PIXELS_connected()
{
    for (Pixel &pixel : this->PIXELS)
    {
        pixel.PIXEL_connection = {};

        for (Pixel &pixel_neighbour : this->PIXELS)
        {
            if (&pixel_neighbour == &pixel)
                continue;

            Pixel top{pixel.x, pixel.y + 1};
            Pixel bottom{pixel.x, pixel.y - 1};
            Pixel right{pixel.x + 1, pixel.y};
            Pixel left{pixel.x - 1, pixel.y};

            if (pixel_neighbour == top)
                pixel.PIXEL_connection.top = &pixel_neighbour;
            else if (pixel_neighbour == bottom)
                pixel.PIXEL_connection.bottom = &pixel_neighbour;
            else if (pixel_neighbour == left)
                pixel.PIXEL_connection.left = &pixel_neighbour;
            else if (pixel_neighbour == right)
                pixel.PIXEL_connection.right = &pixel_neighbour;
        }
    }
}

void Map::check_if_PIXEL_is_NODE()
{
    for (Pixel &pixel : this->PIXELS)
    {
        for (Node node : this->NODES)
        {
            // Si on pixel est référencé comme Node par l'utilisateur, on vérifie si c'est bien le cas.
            if (pixel == node.pixel)
            {
                Connections &NEIGHBOUR{pixel.PIXEL_connection};
                if (pixel.is_VOID || !is_NODE(NEIGHBOUR))
                {
                    std::cout << "le pixel (" << pixel.x << ":" << pixel.y << ") n'est pas un node." << std::endl;
                    exit(EXIT_FAILURE);
                    break;
                }
                pixel.is_NODE = true;
            }
        }
    }
}

// 8) Récupère la tile qui correspond au pixel (en fonction de ses informations)
void Map::get_TILES_path_from_PIXELS()
{
    std::vector<Tile> TILES_list;
    for (Pixel &pixel : this->PIXELS)
    {
        std::vector<std::filesystem::path> TILE_path_list;
        Connections &NEIGHBOUR{pixel.PIXEL_connection};

        if (pixel.is_START_POINT)
        {
            // Point d'entrée
            set_IN_OUT_orientation_texture(NEIGHBOUR, TILE_path_list);
            TILE_path_list.push_back("images/textures/Map/Logo_Empire.png");
        }
        else if (pixel.is_END_POINT)
        {
            // Point de sortie
            set_IN_OUT_orientation_texture(NEIGHBOUR, TILE_path_list);
            TILE_path_list.push_back("images/textures/Map/Logo_Alliance.png");
        }
        else if (pixel.is_PATH) // Point de chemin => route OU virage
        {
            if (pixel.is_NODE) // Virage
                set_NODE_orientation_texture(NEIGHBOUR, TILE_path_list);
            else // Route
                set_PATH_orientation_texture(NEIGHBOUR, TILE_path_list);
        }

        if (!TILE_path_list.empty())
            TILES_list.push_back({pixel, {}, TILE_path_list});
    }
    this->TILES = TILES_list;
}

// 9) Load les textures de la map
void Map::get_TILES_MAP(std::unordered_map<std::filesystem::path, GLuint> &LoadedTextures)
{
    for (Tile &tile : this->TILES)
    {
        for (std::filesystem::path &path : tile.path_list)
        {
            tile.texture_list.push_back(LoadedTextures[path]);
        }
    }
}

// 8) Génération de la map en dessinant via 'draw_quad_with_texture'
void Map::draw_MAP()
{
    for (Tile &tile : this->TILES)
        for (GLuint &texture : tile.texture_list)
            draw_quad_with_texture(texture, tile.pixel.x, tile.pixel.y, *this);
}
