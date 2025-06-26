#include <iostream>
#include "graph.hpp"
#include "mdd.hpp"

int main()
{
    Graph G(10,10);
    G.make_lab();
    auto start=G.draw();
    Position a;
    Position b;
    conflicts c;
    c.collision=false;
    c.follow =ConflictFollow::no_cycle;
    //this examples show that both MDD copy and MDD::get_root work by themselves. Now to figure out why they make the code explode in mapf.
    // MDD M=MDD::fabric_new(*start[1].first,*start[1].second,G.longeur_a_star(*start[1].first,*start[1].second,start[1].second->dist_taxicab_to())-1,G);
    // MDD M2(M);
    // std::cout<<std::endl<<M.get_root()->pos.nth_pos(0)<<std::endl<<std::endl;

    auto solution =G.mapf_icst(start,c);
    auto i=solution.begin();
    while (i!=solution.end())
    {
        std::cout<<*i<<std::endl;
        ++i;
    }
    G.show_mapf_solution(solution);
    return 0;
}
