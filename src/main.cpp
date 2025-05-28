#include <iostream>
#include "graph.hpp"
#include "mdd.hpp"

int main()
{
    /*//test de la fonction check
    std::vector<Position> A{Position(1,1),Position(2,2)};
    std::vector<Position> B{Position(3,3),Position(4,4)};

    std::vector<Position> C{Position(2,2),Position(1,1)};
    std::vector<Position> D{Position(4,4),Position(10,10)};
    conflicts c;
    c.collision=false;
    c.follow =ConflictFollow::no_swap;
    if(check(A,B,C,D,c)){
        std::cout<<"ouir";
    }
    else{
        std::cout<<"nonr";
    }*/
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
