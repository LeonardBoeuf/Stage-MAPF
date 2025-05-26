#include "mdd.hpp"
#include <exception>
#include <algorithm>
#include <map>

Node new_node(std::vector<Position> pos) noexcept {
    Node n;
    n.children = std::set<Node*>();
    n.pos = pos;
    n.valid_path = true;
}

std::vector<Position> neighbours(const Position &pos, const unsigned int width, const unsigned int height) {
    std::vector<Position> positions;

    positions.push_back(pos);
    if (pos.get_x() > 0) positions.push_back(Position(pos.get_x()-1,pos.get_y()));
    if (pos.get_y() > 0) positions.push_back(Position(pos.get_x(),pos.get_y()-1));
    if (pos.get_x()+1 < width) positions.push_back(Position(pos.get_x()+1,pos.get_y()));
    if (pos.get_y()+1 < height) positions.push_back(Position(pos.get_x(),pos.get_y()+1));

    return positions;
}

MDD::MDD(const unsigned int cost, const std::vector<Position> &root) : cost_(cost), root_(root) {}

MDD MDD::fabric_new(
    const std::vector<Position> &start,
    const std::vector<Position> &goal,
    const unsigned int cost,
    const unsigned int g_width,
    const unsigned int g_height
) {
    if (cost == 0) throw std::invalid_argument("Cannot handle 0-cost MDDs");
}

bool check(std::vector<Position> &FromA,
    std::vector<Position> &FromB,
    std::vector<Position> &ToA,
    std::vector<Position> ToB,
    const conflicts &c){
    std::map<Position,int> compteur;
    for (int i = 0; i < ToA.size()+ToB.size(); i++)
    {
        Position current;
        if(i< ToA.size()){
            current=ToA[i];
        }
        else{
            current=ToB[i-ToA.size()];
        }
        compteur[current]=0;
    }
    for (int i = 0; i < ToA.size()+ToB.size(); i++)
    {
        Position current;
        if(i< ToA.size()){
            current=ToA[i];
        }
        else{
            current=ToB[i-ToA.size()];
        }
        compteur[current]++;
        if(compteur[current]>1 and c.collision==false){//conflict de collision
            return false;
        }
    }
    for (int i = 0; i < FromA.size()+FromB.size(); i++)
    {
        Position current;
        if(i< FromA.size()){
            current=FromA[i];
        }
        else{
            current=FromB[i-FromA.size()];
        }
        if(compteur[current]==1){//cas >1 à gerer plus tard : si les collisions sont autorisées mais pas les trains par exemple
            Position other;
            if(i< ToA.size()){
                other=ToA[i];
            }
            else{
                other=ToB[i-ToA.size()];
            }
            if(current!=other and c.follow==ConflictFollow::no_train){//conflict de train

            }
        }
    }
    


}
/*void check(Node &a, Node &b, const conflicts &c={conflict_types::collision}){
    auto i=a.children.begin();
    while(i!=a.children.end()){
        auto j=a.children.begin();
        while(j!=a.children.end()){
            std::map<Position,int> compteur;
            for (int i = 0; i < (**i).pos.size()+(**j).pos.size(); i++)
            {
                Position current;
                if(i< (**i).pos.size()){
                    current=(**i).pos[i];
                }
                else{
                    current=(**j).pos[i-(**i).pos.size()];
                }
                compteur[current]=0;
            }
            for (int i = 0; i < (**i).pos.size()+(**j).pos.size(); i++)
            {
                Position current;
                if(i< (**i).pos.size()){
                    current=(**i).pos[i];
                }
                else{
                    current=(**j).pos[i-(**i).pos.size()];
                }
                compteur[current]++;
            }
            auto y=compteur.begin();

            ++j;
        }
        ++i;
    }
}*/

MDD MDD::cross_prunning(MDD &a, MDD &b,const conflicts &c) noexcept{
    int n;
    if(a.cost_>b.cost_){
        n=a.cost_;
    }
    else {
        n=b.cost_;
    }
    for (int i = 0; i < n; i++)
    {
        
    }
    //MDD ab=fabric_new();

}

