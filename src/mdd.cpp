#include "mdd.hpp"
#include <exception>
#include <algorithm>
#include <map>

std::set<Node*> flag_for_deletion(Node * n){
    std::set<Node*> result;
    std::queue<Node*> file;
    file.push(n);
    while(not file.empty()){
        Node * current=file.front();
        file.pop();
        result.insert(current);
        for(auto i : current->children){//bah oui mais léonard tu sais qu'un noeud peut avoir deux parents donc bon..?
            file.push(i);
        }
    }
    return result;
}

void delete_flaged(std::set<Node*> & file){
    auto i=file.begin();
    while(i!=file.end()){
        Node * current=*i;
        // std::cout<<current->pos<<std::endl;
        delete current;
        ++i;
    }
}

void delete_node(Node * n){
    auto file =flag_for_deletion(n);
    delete_flaged(file);
}

MDD::~MDD(){
    // std::cout<<"Suppression de MDD :"<<std::endl;
    delete_node (root_);
}

MDD::MDD(const MDD& M):cost_(M.cost_) {
    //std::cout<<"constructeur copie faite"<<std::endl;
    root_=copy_node(*(M.root_));
}

Node new_node(const KdimPosition &pos) noexcept {
    return Node {pos, std::set<Node*>(), true};
}

Node* copy_node(const Node & n){
    Node* result=new Node(new_node(n.pos));
    for (auto i :n.children)
    {
        result->children.insert(copy_node(*i));
    }
    return result;    
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

Node* MDD::make_nodes(std::vector<std::vector<Position>> & chemins){
    std::vector<Node*> vus;
    std::vector<Node*> deja_vus;
    for (int i = 0; i < chemins[0].size(); i++)
    {//etape i du chemin (rappel les chemins sont à l'envers)
        for (int j = 0; j < chemins.size(); j++)
        {
            bool trouve=false;
            int a=0;
            while(a<vus.size() and not trouve)
            {
                if(chemins[j][i]==vus[a]->pos.nth_pos(0)){//si on à déjà vu ce chemin à cette étape
                    trouve=true;
                }
                else{
                    ++a;
                }
            }
            if(!trouve){
                // std::cout<<"on crée le noeud de position "<<chemins[j][i]<<std::endl;
                Node* fils=new Node (new_node(chemins[j][i]));
                if(i>0){//si ce n'est pas la dernière étape du chemin :
                    for (int x = 0; x < chemins.size(); x++)
                    {
                        if(chemins[x][i]==chemins[j][i]){// si on part de la même case
                            int y=0;
                            //j'avais oublié la ligne en dessous, causant un problème sur l'ajout. ce problème réglé, on à un double free plus loin.
                            bool re_trouve=false;//comme trouvé mais le nom est déjà pris
                            while(y<deja_vus.size() and not re_trouve)
                            {
                                if(chemins[x][i-1]==deja_vus[y]->pos.nth_pos(0)){//si on à déjà vu ce chemin à l'étape précédente
                                    re_trouve=true;
                                }  
                                else{
                                    ++y;
                                }
                            }
                            if(re_trouve){
                                if(fils->children.count(deja_vus[y])==0){
                                    fils->children.insert(deja_vus[y]);//en cas de deux chemins différant partant et finissant au même endroit, l'insertion sera faite de fois, mais en théorie c fine parce que c un set donc l'insert remarque qu'il là déjà
                                    // std::cout<<"ajout de"<<deja_vus[y]->pos.nth_pos(0)<<". Y'en a mtn : "<<fils->children.size()<<std::endl;
                                }
                            }
                            else{
                                throw std::runtime_error("azroj)e");
                            }
                        }
                    }
                }
                vus.push_back(fils);
            }
        }
        // std::cout<<"nb d'objets dans vus : "<<vus.size()<<" : ";
        // for (int w = 0; w < vus.size(); w++)
        // {
        //     std::cout<<vus[w]->pos.nth_pos(0);
        // }
        // std::cout<<std::endl;
        deja_vus=vus;
        // std::cout<<"nb d'objets dans deja_vus : "<<vus.size()<<std::endl;
        vus.clear();
    }
    //vérifier que déjà_vus soit bien composé d'un seul élément ?
    return deja_vus[0];
}

int MDD::get_cost() const{
    return cost_;
}

int MDD::get_dim() const{
    return root_->pos.get_dim_k();
}

Node* MDD::get_root() const{
    return copy_node(*root_);
}

bool check(KdimPosition &FromA,
    KdimPosition &FromB,
    KdimPosition &ToA,
    KdimPosition ToB,
    const conflicts &c){
    std::map<Position,int> compteur;
    for (int i = 0; i < ToA.get_dim_k()+ToB.get_dim_k(); i++)
    {
        Position current;
        if(i< ToA.get_dim_k()){
            current=ToA.nth_pos(i);
        }
        else{
            current=ToB.nth_pos(i-ToA.get_dim_k());
        }
        compteur[current]=0;
    }
    for (int i = 0; i < ToA.get_dim_k()+ToB.get_dim_k(); i++)
    {
        Position current;
        if(i< ToA.get_dim_k()){
            current=ToA.nth_pos(i);
        }
        else{
            current=ToB.nth_pos(i-ToA.get_dim_k());
        }
        compteur[current]++;
        if(compteur[current]>1 and c.collision==false){//conflict de collision
            // std::cout<<"Conflict de collision entre :"<<std::endl;
            // std::cout<<FromA<<"|"<<FromB<<std::endl;
            // std::cout<<ToA<<"|"<<ToB<<std::endl;
            return false;
        }
    }
    Position current;
    Position other;
    for (int i = 0; i < FromA.get_dim_k()+FromB.get_dim_k(); i++)
    {
        if(i< FromA.get_dim_k()){
            current=FromA.nth_pos(i);
        }
        else{
            current=FromB.nth_pos(i-FromA.get_dim_k());
        }
        bool train=false;
        if(compteur[current]>1){//conflict de train
            train=true;
        }
        else if(compteur[current]==1){//attention : other ne fonctionne pas dans ce cas là.
            if(i< ToA.get_dim_k()){
                other=ToA.nth_pos(i);
            }
            else{
                other=ToB.nth_pos(i-ToA.get_dim_k());
            }
            if(current!=other ){//conflict de train
                train=true;
            }
        }
        if(train){
            if(c.follow==ConflictFollow::no_train){
                // std::cout<<"Conflict de train entre :"<<std::endl;
                // std::cout<<FromA<<"|"<<FromB<<std::endl;
                // std::cout<<ToA<<"|"<<ToB<<std::endl;
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
                    while ( x < ToA.get_dim_k()+ToB.get_dim_k() and other!=current)//trouver sur quelle position atteris notre current
                    {
                        //std::cout<<"x : "<<x<<", toA.size : "<<ToA.size()<<", toB.size : "<<ToB.size()<<std::endl;
                        if(x!=pos_cur){
                            if(x< ToA.get_dim_k()){
                                other=ToA.nth_pos(x);
                            }
                            else{
                                other=ToB.nth_pos(x-ToA.get_dim_k());
                            }
                        }
                        ++x;
                    }
                    if(other!=current and x==ToA.get_dim_k()+ToB.get_dim_k()){//on a pas trouvé : on arrète de parcourir
                        //std::cout<<"on a pas trouvé"<<std::endl;
                        continuer=false;
                    }
                    else{
                        x--;
                        if(x< FromA.get_dim_k()){
                            current=FromA.nth_pos(x);
                        }
                        else{
                            current=FromB.nth_pos(x-FromA.get_dim_k());
                        }
                        pos_cur=x;
                        //on recommence la boucle à partir de la nouvelle case
                    }
                    
                }
                if(continuer){//conflict de cycle
                    //std::cout<<"taille du cycle :"<<visites.size()<<std::endl;
                    if(c.follow==ConflictFollow::no_cycle){
                        // std::cout<<"Conflict de cycle entre :"<<std::endl;
                        // std::cout<<FromA<<"|"<<FromB<<std::endl;
                        // std::cout<<ToA<<"|"<<ToB<<std::endl;
                        return false;
                    }
                    else if(visites.size()==2){//conflict de swap
                        if(c.follow==ConflictFollow::no_swap){
                            // std::cout<<"Conflict de swap entre :"<<std::endl;
                            // std::cout<<FromA<<"|"<<FromB<<std::endl;
                            // std::cout<<ToA<<"|"<<ToB<<std::endl;
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void MDD::cross_prunning(MDD &a, MDD &b,const conflicts &c) noexcept{
    std::map<Position,int> compteur;
    // //test de collision à la racine.
    // std::cout<<"debut de prunning"<<std::endl;
    for (int i = 0; i < a.root_->pos.get_dim_k(); i++)
    {
        Position current;
        current=a.root_->pos.nth_pos(i);
        compteur[current]=0;
    }
    for (int i = 0; i < b.root_->pos.get_dim_k(); i++)
    {
        Position current;
        current=b.root_->pos.nth_pos(i);
        compteur[current]=0;
    }
    for (int i = 0; i < a.root_->pos.get_dim_k(); i++)
    {
        Position current;
        current=a.root_->pos.nth_pos(i);
        compteur[current]++;
        if(compteur[current]>1 and c.collision==false){//conflict de collision à la racine
            a.root_->valid_path=false;
            b.root_->valid_path=false;
        }
    }
    for (int i = 0; i < b.root_->pos.get_dim_k(); i++)
    {
        Position current;
        current=b.root_->pos.nth_pos(i);
        compteur[current]++;
        if(compteur[current]>1 and c.collision==false){//conflict de collision à la racine
            a.root_->valid_path=false;
            b.root_->valid_path=false;
        }
    }
    if(a.root_->valid_path){//si on a pas déjà exclu la racine :
        cross_prunning_rec(*a.root_,*b.root_,c);
    }
}

void MDD::cross_prunning_rec(Node &a, Node &b,const conflicts &c) noexcept{
    if(a.children.size()==0 and b.children.size()!=0){//on a finit a mais pas b
        unsigned int nb_conflicts(0);
        for (auto j : b.children) {
                if(check(a.pos,b.pos,a.pos,j->pos,c)==false){//le chemin n'est pas valide
                    ++nb_conflicts;
                    //a.valid_path=false;
                    j->valid_path=false;//dans ce cas là on est en conflict avec TOUS les enfants de a : le seul
                }
                else
                {
                    cross_prunning_rec(a,*j,c);
                }
        }
        if(nb_conflicts==b.children.size()){//si on est en conflict avec TOUS les enfants en face,
            a.valid_path=false;//on invalide le chemin
        }
    }
    else if(b.children.size()==0 and a.children.size()!=0){//on a finit b mais pas a
        unsigned int nb_conflicts(0);
        for (auto j : a.children) {
                if(check(a.pos,b.pos,j->pos,b.pos,c)==false){//le chemin n'est pas valide
                    ++nb_conflicts;
                    //a.valid_path=false;
                    j->valid_path=false;//dans ce cas là on est en conflict avec TOUS les enfants de a : le seul
                }
                else
                {
                    cross_prunning_rec(*j,b,c);
                }
        }
        if(nb_conflicts==a.children.size()){//si on est en conflict avec TOUS les enfants en face,
            b.valid_path=false;//on invalide le chemin
        }
    }
    else{
        std::vector<unsigned int> nb_conflicts_a((a.children.size()),0);
        std::vector<unsigned int> nb_conflicts_b((b.children.size()),0);
        int x(0);
        for (auto i : a.children) {
            int y(0);
            for (auto j : b.children) {
                if(check(a.pos,b.pos,i->pos,j->pos,c)==false){//le chemin n'est pas valide
                    nb_conflicts_a[x]++;
                    nb_conflicts_b[y]++;
                }
                else
                {
                    cross_prunning_rec(*i,*j,c);
                }
                ++y;
            }
            if(nb_conflicts_a[x]==b.children.size()){//si on est en conflict avec TOUS les enfants en face,
                i->valid_path=false;//on invalide le chemin
            }
            ++x;
        }
        int y(0);
        for (auto j : b.children) {
            if(nb_conflicts_b[y]==a.children.size()){//si on est en conflict avec TOUS les enfants en face,
                j->valid_path=false;//on invalide le chemin
            }
            ++y;
        }
    }
    //propagation vers le haut de l'invalidité
    unsigned int nb_invalides(0);
    for (auto i : a.children) {
        if(i->valid_path==false){
            ++nb_invalides;
        }
        else break;
    }
    if(nb_invalides==a.children.size() and nb_invalides!=0){//si tous les enfants sont invalides (et qu'on a des enfants)
        a.valid_path=false;//le parent aussi
    }
    nb_invalides=0;
    for (auto i : b.children) {
        if(i->valid_path==false){
            ++nb_invalides;
        }
        else break;
    }
    if(nb_invalides==b.children.size() and nb_invalides!=0){//si tous les enfants sont invalides
        b.valid_path=false;//le parent aussi
    }
}


MDD MDD::fusion(const MDD &a,const MDD &b,const conflicts &c) {
    if(a.root_->valid_path and b.root_->valid_path){//sinon c'est déjà fini
        std::map<Position,int> compteur;
        //test de collision à la racine.
        for (int i = 0; i < a.root_->pos.get_dim_k(); i++)
        {
            Position current;
            current=a.root_->pos.nth_pos(i);
            compteur[current]=0;
        }
        for (int i = 0; i < b.root_->pos.get_dim_k(); i++)
        {
            Position current;
            current=b.root_->pos.nth_pos(i);
            compteur[current]=0;
        }
        for (int i = 0; i < a.root_->pos.get_dim_k(); i++)
        {
            Position current;
            current=a.root_->pos.nth_pos(i);
            compteur[current]++;
            if(compteur[current]>1 and c.collision==false){//conflict de collision à la racine
                throw std::invalid_argument("agents share starting position but collisions aren't allowed"); 
            }
        }
        for (int i = 0; i < b.root_->pos.get_dim_k(); i++)
        {
            Position current;
            current=b.root_->pos.nth_pos(i);
            compteur[current]++;
            if(compteur[current]>1 and c.collision==false){//conflict de collision à la racine
                throw std::invalid_argument("agents share starting position but collisions aren't allowed"); 
            }
        }
        int n;
        if(a.cost_>b.cost_){
            n=a.cost_;
        }
        else{
            n=b.cost_;
        }
        std::pair<int,Node> paire =fusion_rec(*a.root_,*b.root_,c,n);
        if(paire.first){//si la profondeur du MDD correspond au cout, cad si la fusion est un succés. Dans le cas contraire, il faut passer au prochain icst
            Node* racine =new Node(paire.second);
            // std::cout<<"avant return : "<<nouveau.get_dim()<<std::endl;
            return MDD (n, racine);
        }
        else{
            std::string message = "Fusion has failed because MDD is of length "+std::to_string(paire.first)+" when it should be of lenght "+std::to_string(n);
            throw std::runtime_error(message);
        }
    }
    else{
        throw std::runtime_error("Fusion has failed");
    }
}

std::pair<bool,Node> MDD::fusion_rec(Node &a, Node &b,const conflicts &c,int prof) noexcept{
    bool solution=(prof==0);//n est la profondeur maximale atteinte.
    Node nouveau=new_node(KdimPosition::merge(a.pos,b.pos));
    // std::cout<<nouveau.pos.get_dim_k()<<std::endl;
    // std::cout<<a.pos.get_dim_k()+b.pos.get_dim_k()<<std::endl;
    if(a.children.size()==0 and b.children.size()!=0){//on a finit a mais pas b
        for (auto j : b.children) {
            if(j->valid_path){//si le chemin n'as pas été invalidé plus tôt
                if(check(a.pos,b.pos,a.pos,j->pos,c))
                {
                    std::pair<bool,Node> paire =fusion_rec(a,*j,c,prof-1);
                    if(paire.first){//si la fusion atteint bien la solution :
                        solution=true;
                        Node* fils= new Node(paire.second);
                        nouveau.children.insert(fils);
                    }
                }
            }
        }
    }
    else if(b.children.size()==0 and a.children.size()!=0){//on a finit b mais pas a
        for (auto j : a.children) {
            if(j->valid_path){//si le chemin n'as pas été invalidé plus tôt
                if(check(a.pos,b.pos,j->pos,b.pos,c))
                {
                    std::pair<bool,Node> paire =fusion_rec(*j,b,c,prof-1);
                    if(paire.first){//si la fusion atteint bien la solution :
                        solution=true;
                        Node* fils= new Node(paire.second);
                        nouveau.children.insert(fils);
                    }
                }
            }
        }
    }
    else{
        // std::cout<<"a a "<<a.children.size()<<" enfants"<<std::endl;
        for (auto i : a.children) {
            if(i->valid_path){//si le chemin n'as pas été invalidé plus tôt
                for (auto j : b.children) {
                    if(j->valid_path){//si le chemin n'as pas été invalidé plus tôt
                        // std::cout<<"Tentative de fusion entre :"<<std::endl;
                        // std::cout<<a.pos<<"|"<<b.pos<<std::endl;
                        // std::cout<<i->pos<<"|"<<j->pos<<std::endl;
                        if(check(a.pos,b.pos,i->pos,j->pos,c))
                        {
                            std::pair<bool,Node> paire =fusion_rec(*i,*j,c,prof-1);
                            if(paire.first){//si la fusion atteint bien la solution :
                                solution=true;
                                Node* fils= new Node(paire.second);
                                nouveau.children.insert(fils);
                            }
                        }
                    }
                }
            }
            
        }
    }
    // std::cout<<"Profondeur : "<<n<<std::endl;
    return std::pair<bool,Node> (solution,nouveau);
}

std::vector<KdimPosition> MDD::output(){//les mdds avec plus de deux options ont comme position de départ 42103351 apparement (provient probablement de fusion)
    std::vector<KdimPosition> resultat;
    Node* current = root_;
    //std::cout<<root_->pos.nth_pos(0)<<std::endl;
    // while (current->children.size()!=0)//ceci nous fait nous arreter juste avant la fin
    // {
    //     resultat.push_back(current->pos);
    //     current=*current->children.begin();
    // }
    // resultat.push_back(current->pos);
    resultat.push_back(current->pos);
    for (int i = 1; i <=cost_; i++)
    {
        if(current->children.begin()==current->children.end()){
            std::cout<<"je l'savais"<<std::endl;
        }
        current=*current->children.begin();
        resultat.push_back(current->pos);
        std::cout<<current->pos<<std::endl;
    }
    return resultat;
}

void MDD::operator=(const MDD & M){
    //std::cout<<"= copie faite"<<std::endl;
    cost_=M.cost_;
    root_=copy_node(*M.root_);
}