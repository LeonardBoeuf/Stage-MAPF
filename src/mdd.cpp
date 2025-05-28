#include "mdd.hpp"
#include <exception>
#include <algorithm>
#include <map>

Node new_node(const KdimPosition &pos) noexcept {
    return Node {pos, std::set<Node*>(), true};
}

std::vector<Position> neighbours(const Position &pos, const unsigned int width, const unsigned int height) {
    std::vector<Position> positions;

    if (pos.get_x() > 0) positions.push_back(Position(pos.get_x()-1,pos.get_y()));
    if (pos.get_y() > 0) positions.push_back(Position(pos.get_x(),pos.get_y()-1));
    if (pos.get_x()+1 < width) positions.push_back(Position(pos.get_x()+1,pos.get_y()));
    if (pos.get_y()+1 < height) positions.push_back(Position(pos.get_x(),pos.get_y()+1));
    positions.push_back(pos);

    return positions;
}

MDD::MDD(const unsigned int cost, Node* const root) : cost_(cost), root_(root) {}

MDD MDD::fabric_new(
    const KdimPosition &start,
    const KdimPosition &goal,
    const unsigned int cost,
    const unsigned int g_width,
    const unsigned int g_height
) {
    if (cost == 0) {
        if (start != goal) throw std::invalid_argument("No such MDD");
        else return MDD(cost, new Node(new_node(KdimPosition(start))));
    }
    
    Node root(new_node(start));
    std::vector<std::vector<KdimPosition>> present_positions(cost+1);
    unsigned int current_cost(1);
    while (current_cost < cost) {
        for (const auto &agent : present_positions[present_positions.size()-1]) {
            
        }
    }


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
    Position current;
    Position other;
    for (int i = 0; i < FromA.size()+FromB.size(); i++)
    {
        if(i< FromA.size()){
            current=FromA[i];
        }
        else{
            current=FromB[i-FromA.size()];
        }
        bool train=false;
        if(compteur[current]>1){//conflict de train
            train=true;
        }
        else if(compteur[current]==1){//attention : other ne fonctionne pas dans ce cas là.
            if(i< ToA.size()){
                other=ToA[i];
            }
            else{
                other=ToB[i-ToA.size()];
            }
            if(current!=other ){//conflict de train
                train=true;
            }
        }
        if(train){
            if(c.follow==ConflictFollow::no_train){
                return false;
            }
            else{
                //on a un train qui commence à current. Maintenant, identifier si on à un cycle.
                //voir si other n'est pas aussi un cas de train.
                std::set<Position> visites;
                bool continuer=true;
                int pos_cur=i;
                while (continuer and visites.count(current)==0)//ne fonctionne pas.
                {
                    //std::cout<<"current : "<<current<<", pos_cur : "<<pos_cur<<std::endl;
                    visites.insert(current);//on à visité current
                    int x = 0;
                    while ( x < ToA.size()+ToB.size() and other!=current)//trouver sur quelle position atteris notre current
                    {
                        //std::cout<<"x : "<<x<<", toA.size : "<<ToA.size()<<", toB.size : "<<ToB.size()<<std::endl;
                        if(x!=pos_cur){
                            if(x< ToA.size()){
                                other=ToA[x];
                            }
                            else{
                                other=ToB[x-ToA.size()];
                            }
                        }
                        ++x;
                    }
                    if(other!=current and x==ToA.size()+ToB.size()){//on a pas trouvé : on arrète de parcourir
                        //std::cout<<"on a pas trouvé"<<std::endl;
                        continuer=false;
                    }
                    else{
                        x--;
                        if(x< FromA.size()){
                            current=FromA[x];
                        }
                        else{
                            current=FromB[x-FromA.size()];
                        }
                        pos_cur=x;
                        //on recommence la boucle à partir de la nouvelle case
                    }
                    
                }
                if(continuer){//conflict de cycle
                    //std::cout<<"taille du cycle :"<<visites.size()<<std::endl;
                    if(c.follow==ConflictFollow::no_cycle){
                        return false;
                    }
                    else if(visites.size()==2){//conflict de swap
                        if(c.follow==ConflictFollow::no_swap){
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
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
