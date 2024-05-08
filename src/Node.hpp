#pragma once

struct Node
{
    int id{0};
    int x{0};
    int y{0};
    int connected_to{0};
    std::vector<Node> get_nodes_from_itd();
};