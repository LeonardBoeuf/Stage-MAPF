#include <iostream>
#include "graph.hpp"

int main()
{
    Graph G(100,100);
    G.make_lab();
    auto start=G.draw();
    Position a;
    Position b;
    for(int i=0;i<10;++i){
        if(start[i].first !=nullptr){
            a=*start[i].first;
            if(start[i].second !=nullptr){
                b=*start[i].second;
                G.show_path(a,b,b.dist_taxicab_to());
                G.show_thoughts(a,b,b.dist_taxicab_to());
            }
            else {
                b= Position(0,0);//throw instead ?
            }
        }
    }
    return 0;    
}
