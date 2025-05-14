#include <iostream>
#include "graphismes.hh"

int main()
{
    Grille_MAPF M(5,4);
    M.new_wall(1,2);
    M.run();
}
