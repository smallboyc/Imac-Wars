#include <iostream>
#include "Node.hpp"
#include <fstream>
#include <vector>

std::vector<Node> Node::get_nodes_from_itd()
{
    std::vector<Node> data_nodes{};
    // Open the input file named "input.txt"
    std::ifstream inputFile("../../data/map.itd");

    std::string line;

    while (getline(inputFile, line))
    {
        // Récupération des paramètres des nodes.
        if (line.find("node") != std::string::npos)
        {
            std::string node_str{""};
            for (char c : line)
                if (isdigit(c))
                    node_str += c;

            Node new_node{node_str[0] - '0', node_str[1] - '0', node_str[2] - '0', node_str[3] - '0'};
            data_nodes.push_back(new_node);
        }
    }
    inputFile.close();

    return data_nodes;
}