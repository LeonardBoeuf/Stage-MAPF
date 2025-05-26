#include "mdd.hpp"
#include <exception>
#include <algorithm>

std::vector<Position> neighbours(const Position &pos, const unsigned int width, const unsigned int height) {
    std::vector<Position> positions;

    positions.push_back(pos);
    if (pos.get_x() > 0) positions.push_back(Position(pos.get_x()-1,pos.get_y()));
    if (pos.get_y() > 0) positions.push_back(Position(pos.get_x(),pos.get_y()-1));
    if (pos.get_x()+1 < width) positions.push_back(Position(pos.get_x()+1,pos.get_y()));
    if (pos.get_y()+1 < height) positions.push_back(Position(pos.get_x(),pos.get_y()+1));

    return positions;
}

MDD::MDD(
    const Position &start,
    const Position &goal,
    const unsigned int cost,
    const unsigned int g_width,
    const unsigned int g_height
) : start_(start), cost_(cost), diagram_() {
    if (cost == 0) throw std::invalid_argument("Cannot handle 0-cost MDDs");
    if (cost == 1) {
        auto nb (neighbours(start,g_width,g_height));
        if (std::find(nb.begin(),nb.end(),goal) == nb.end()) throw std::invalid_argument("No path to goal with such cost");
        Node n {true, std::vector<Position>()};
        std::map<Position, Node> m {{goal,n}};
        diagram_.push_back(m);
    }

    unsigned int current_cost(1);
    while (current_cost < cost) {
        
    }
}

std::pair<std::vector<bool>,std::vector<bool>> MDD::prunning_step(Position FromA, Node ToA ,Position FromB, Node ToB) noexcept{
    auto ret=std::pair<std::vector<bool>,std::vector<bool>>((true,ToA.children.size()),(true,ToB.children.size()));
    for(int i=0;i<ToA.children.size();++i){
        if(ToA.children[i]==FromB /*and conflicts train interdits*/){
            //train conflict
            ret.first[i]=false;
            /* gestion des swap si un train est autorisé
                for(int j=0;j<ToB.children.size();++j){
                    if(ToA.children[i]==FromB){
                        //swap conflict
                        ret.first[i]=false;
                        ret.second[j]=false;
                    }
                }
            */
        }
        for(int j=0;j<ToB.children.size();++j){
            if(ToA.children[i]==ToB.children[i]){
                
            }
        }
    }
}


bool MDD::cross_prunning(MDD &a, MDD &b) noexcept{
    if(a.start_==b.start_){
        return false;
    }
    int n;
    if(a.diagram_.size()>b.diagram_.size()){
        n=a.diagram_.size();
        /*Position goal = b.diagram_[b.diagram_.size()-1].begin()->first;
        for(int i=b.diagram_.size();i<n;++i){
            Node n {true, std::vector<Position>()};
            std::map<Position, Node> m {{goal,n}};
            b.diagram_.push_back(m);
        }*/
    }
    int i=0;
    bool chemin= true;
    while(i<n and chemin==true){
        
        ++i;
    }
}
