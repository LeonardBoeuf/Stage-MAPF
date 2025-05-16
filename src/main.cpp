#include <iostream>
#include "graph.hpp"

int main()
{
    Graph G(50,50);
    G.make_lab();
    auto start=G.draw();
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
    G.show_path(a,b,[&b](const Position&pos){return pos.dist_eucl(b);});
    G.show_thoughts(a,b,[&b](const Position&pos){return pos.dist_eucl(b);});
    return 0;
}
