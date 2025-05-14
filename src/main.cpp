#include <iostream>
#include "graph.hpp"

int main()
{
    Graph G(50,40);
    G.run(Position(5,5),Position(2,3),[](const Position&pos){return pos.get_x()*pos.get_x()+pos.get_y()*pos.get_y();});
    G.run(Position(0,0),Position(2,30),[](const Position&pos){return pos.get_x()*pos.get_x()+pos.get_y()*pos.get_y();});

}
