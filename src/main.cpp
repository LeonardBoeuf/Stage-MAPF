#include <iostream>
#include "graph.hpp"

int main()
{
    Graph G(100,100);
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
    G.show_path(a,b,b.dist_taxicab_to());
    G.show_thoughts(a,b,b.dist_taxicab_to());
    return 0;
}
