#pragma once
#include <vector>
#include <unordered_map>
#include <utility>

namespace Graph
{
    struct WeightedGraphEdge
    {
        int to{};
        float weight{1.0f};

        bool operator==(WeightedGraphEdge const &other) const = default;
        bool operator!=(WeightedGraphEdge const &other) const = default;
    };

    struct WeightedGraph
    {
        std::unordered_map<int, std::vector<WeightedGraphEdge>> adjacency_list{};

        void add_vertex(int const id);

        void add_directed_edge(int const from, int const to, float const weight);

        bool operator==(WeightedGraph const &other) const = default;
        bool operator!=(WeightedGraph const &other) const = default;

        void print_BFS(int const start) const;
        // Dijkstra
        std::unordered_map<int, std::pair<float, int>> dijkstra(int const &start, int const end);
    };
}
