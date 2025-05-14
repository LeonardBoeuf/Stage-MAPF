#pragma once
#include <string>
#include "graph.hpp"


class Grille_MAPF{
    public :
    Grille_MAPF(unsigned int width, unsigned int height) : _graphe(Graph(width,height)){}
    void run();
    void new_wall(const unsigned int x, const unsigned int y);
    void make_lab();

    private :
    Graph _graphe;
};