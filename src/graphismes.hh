#pragma once
#include <string>
#include "graph.hpp"


class Grille_MAPF{
    public :
    Grille_MAPF(unsigned int width, unsigned int height) : _graphe(Graph(width,height)){}
    void run();

    private :
    Graph _graphe;
};