#include <iostream>
#include "graph.hpp"

int main()
{
    Graph G(5,4);
    G.new_wall(1,2);
    G.new_agent(0,0,1);
    G.run();
}
