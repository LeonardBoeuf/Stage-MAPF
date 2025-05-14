#include <iostream>
#include "graph.hpp"

int main()
{
    Graph G(50,40);
    auto start=G.run();
    Position a;
    Position b;
    if(start.first ==nullptr){
        a= Position(0,0);
    }
    else {
        a=*start.first;
    }
    if(start.second ==nullptr){
        b= Position(0,0);
    }
    else {
        b=*start.second;
    }
    G.run(a,b,[&b](const Position&pos){return pos.dist_eucl(b);});

}
