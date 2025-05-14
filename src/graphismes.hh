#pragma once
#include <string>
#include "graph.hpp"


class Grille_MAPF{
    public :
    Grille_MAPF(Graph & g) : _graphe(g){}
    void run();

    private :
    Graph _graphe;
};