#pragma once
#include <string>
#include "graph.hh"

/*class Grille_MAPF{
    public :
    Grille_MAPF(Graph & g) : _graphe(g){}

    private :
    Graph _graphe;
};*/

class Grille_MAPF{
    public :
    Grille_MAPF(int n) : _n(n){}
    void run();

    private :
    int _n;
};