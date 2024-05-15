#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <queue>
#include <utility>
#include "Graph.hpp"

namespace Graph
{
    void WeightedGraph::add_vertex(int const id)
    {
        // Si le sommet n'existe pas encore, alors on le créer.
        if (this->adjacency_list.find(id) == this->adjacency_list.end())
            adjacency_list.insert({id, {}});
    }

    void WeightedGraph::add_directed_edge(int const from, int const to, float const weight)
    {
        // On s'assure d'avoir les deux sommets pour créer une edge.
        add_vertex(from);
        add_vertex(to);
        // On cible le sommet "from" auquel on ajoute le sommet "to" et le poids de l'edge.
        for (auto &element : this->adjacency_list)
        {
            if (element.first == from)
            {
                element.second.push_back({to, weight});
                break;
            }
        }
        // std::cout << "Edge de " << from << " vers " << to << " créée avec succès." << std::endl;
    }

    WeightedGraph build_from_adjacency_matrix(std::vector<std::vector<float>> const &adjacency_matrix)
    {
        WeightedGraph graph;
        for (size_t i{0}; i < adjacency_matrix.size(); i++)
            for (size_t j{0}; j < adjacency_matrix.size(); j++)
                if (adjacency_matrix[i][j] != 0)
                    graph.add_directed_edge(i, j, adjacency_matrix[i][j]);
        return graph;
    }


    void WeightedGraph::print_BFS(int const start) const
    {
        std::queue<int> queue;
        std::vector<int> already_checked_ids;
        int current{start};
        queue.push(current);
        while (!queue.empty())
        {
            std::cout << queue.front() << std::endl;
            current = queue.front();
            queue.pop();
            for (auto tos : this->adjacency_list.at(current))
            {
                if (std::find(already_checked_ids.begin(), already_checked_ids.end(), tos.to) == already_checked_ids.end())
                {
                    queue.push(tos.to);
                    already_checked_ids.push_back(tos.to);
                }
            }
        }
    }

    std::unordered_map<int, std::pair<float, int>> dijkstra(WeightedGraph const &graph, int const &start, int const end)
    {
        std::unordered_map<int, std::pair<float, int>> distances{};
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> to_visit{};

        std::vector<int> visited;

        // 1. On ajoute le sommet de départ à la liste des sommets à visiter avec une distance de 0 (on est déjà sur le sommet de départ)
        to_visit.push({0.f, start});
        // Tant qu'il reste des sommets à visiter
        while (!to_visit.empty())
        {
            // 2. On récupère le sommet le plus proche du sommet de départ dans la liste de priorité to_visit
            int current_node{to_visit.top().second};
            float current_distanceToStart{to_visit.top().first};
            to_visit.pop();
            // 3.Si on atteind le point d'arrivé, on s'arrête
            if (current_node == end)
                return distances;
            // 3. On parcourt la liste des voisins (grâce à la liste d'adjacence) du noeud courant
            for (auto edge : graph.adjacency_list.at(current_node))
            {
                // 4. on regarde si le noeud existe dans le tableau associatif (si oui il a déjà été visité)
                int current_adj_node{edge.to};
                float current_adj_distanceToParent{edge.weight};
                if (std::find(visited.begin(), visited.end(), current_adj_node) == visited.end())
                {
                    // 5. Si le noeud n'a pas été visité, on l'ajoute au tableau associatif en calculant la distance pour aller jusqu'à ce noeud
                    // la distance actuelle + le point de l'arrête)
                    float adj_distanceToStart = current_distanceToStart + current_adj_distanceToParent;
                    if (distances.find(current_adj_node) == distances.end())
                        distances.insert({current_adj_node, {adj_distanceToStart, current_node}});
                    else if (distances.at(current_adj_node).first > adj_distanceToStart)
                        distances[current_adj_node] = {adj_distanceToStart, current_node};

                    // 6. On ajoute également le noeud de destination à la liste des noeud à visité (avec la distance également pour prioriser les noeuds les plus proches)
                    to_visit.push({adj_distanceToStart, current_adj_node});
                }
            }
            visited.push_back(current_node);
        }
        return distances;
    }

}
