#include <iostream>
#include "graph.hpp"

int main()
{
    Graph G(50,40);
    G.run(Position(5,5),Position(2,3),Position(2,3).dist_taxicab_to());
    G.run(Position(0,0),Position(2,30),Position(2,30).dist_taxicab_to());

}
