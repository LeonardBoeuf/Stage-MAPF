#include "graph.hpp"
#include <queue>
#include <map>
#include <algorithm>
#include <limits>
#include <iostream>
#include <random>
#include <ctime>
#include "priority_queue.hh"
#include "icst.hpp"
float framerate=900;
int etapes_par_frames = 4;


Graph::Graph(unsigned int width, unsigned int height):
    width_(width),
    height_(height),
    grille_(height_)
{
    for (unsigned int i(0); i<height_; ++i) {
        grille_.push_back(std::vector<std::unique_ptr<Cell>>(width_));
        for (unsigned int j(0); j<width_; ++j) {
            grille_[i].push_back(nullptr);
        }
    }
}

bool Graph::is_empty(const Position &pos) const {
    return grille_[pos.get_y()][pos.get_x()] == nullptr;
    // return grille_.at(pos.get_y()).at(pos.get_x()) == nu;
}

// int Graph::get_agent_id(const Position &pos) const{
//     // return grille_[pos.get_y()][pos.get_x()] == nullptr;
//     return ;
// }

// Position Graph::get_agent_pos(const int id) const{
//     int i=0;
//     int j=0;
//     bool trouve=false;
//     while (i<width_)
//     {
//         while (j<height_)
//             {
//                 if(Graph::get_agent_id(Position(i,j))==id){
//                     return Position(i,j);
//                 }
//                 ++j;
//             }
//         ++i;
//     }
//     throw std::exception();
    
// }

// void Graph::move_agent(const Position &from, const Position &to){
//     if(Graph::is_agent(from)){
//         int id=Graph::get_agent_id(from);
//         Graph::set_empty(from);
//         Graph::new_agent(to,id);
//     }
// }

unsigned int Graph::get_width() const noexcept {
    return width_;
}

unsigned int Graph::get_height() const noexcept {
    return height_;
}

void Graph::new_wall(const Position &pos) {
    Graph::new_wall(pos.get_x(),pos.get_y());
}

void Graph::new_wall(const unsigned int x, const unsigned int y) {
    if (x >= width_ || y >= height_) throw std::out_of_range("Tried to place outside the graph");
    grille_[y][x] = std::make_unique<Wall>();
}

bool Graph::is_wall(const Position &pos) const {
    //std::cout<<"is_wall ("<<pos.get_x()<<", "<<pos.get_y()<<")"<<std::endl;
    if (grille_.at(pos.get_y()).at(pos.get_x()) == nullptr) return false;
    auto p(dynamic_cast<Wall *>(grille_[pos.get_y()][pos.get_x()].get()));
    return p != nullptr;
}

void Graph::set_empty(const Position &pos) {
    Graph::set_empty(pos.get_x(),pos.get_y());
}

void Graph::set_empty(const unsigned int x, const unsigned int y) {
    if (x >= width_ || y >= height_) throw std::out_of_range("Tried to place outside the graph");
    grille_[y][x] = nullptr;
}

bool Graph::is_agent(const Position &pos) const {
    if (grille_.at(pos.get_y()).at(pos.get_x()) == nullptr) return false;
    auto p(dynamic_cast<Agent *>(grille_[pos.get_y()][pos.get_x()].get()));
    return p != nullptr;
}

void Graph::new_agent(const Position &pos, const int id) {
    Graph::new_agent(pos.get_x(),pos.get_y(),id);
}

void afficher_liste_chemins(const std::vector<std::vector<Position>> & c){
    auto i = c.begin();
    while (i!=c.end())
    {
        auto j=(*i).end();
        j--;
        while (j!=(*i).begin())
        {
            std::cout<<*j;
            --j;
            std::cout<<"|";
        }
        std::cout<<*j<<std::endl;
        ++i;
    }
    
    
}

std::vector<std::vector<Position>> Graph::chemins_taille_n(const Position &start, const Position &goal, unsigned int n) const{
    std::vector<std::vector<Position>> chemins;
    if (n == 0) {
        if (start == goal){
            std::vector<Position> chemin;
            chemin.push_back(start);
            chemins.push_back(chemin);
        }
    }
    else{
        std::vector<Position> next;
        next.push_back(Position(start.get_x(),start.get_y()));
        if ((start.get_x() +1 < width_) && (is_wall(Position(start.get_x()+1,start.get_y()))==false)) {
            next.push_back(Position(start.get_x()+1,start.get_y()));
        }
        if ((start.get_y() +1 < height_) && (is_wall(Position(start.get_x(),start.get_y()+1))==false)) {
            next.push_back(Position(start.get_x(),start.get_y()+1));
        }
        if ((start.get_x() >0) && (is_wall(Position(start.get_x()-1,start.get_y()))==false)) {
            next.push_back(Position(start.get_x()-1,start.get_y()));
        }
        if ((start.get_y() >0) && (is_wall(Position(start.get_x(),start.get_y()-1))==false)) {
            next.push_back(Position(start.get_x(),start.get_y()-1));
        }
        for(int i=0;i<next.size();++i){
            std::vector<std::vector<Position>> nouveaux;
            nouveaux=chemins_taille_n(next[i],goal,n-1);
            auto j=nouveaux.begin();
            while (j!=nouveaux.end())
            {
                (*j).push_back(start);
                chemins.push_back(*j);
                ++j;
            }
        }
    }
    return chemins;
}

MDD MDD::fabric_new(
    const Position &start,
    const Position &goal,
    const unsigned int cost,
    const Graph & g
) {
    std::vector<std::vector<Position>> chemins=g.chemins_taille_n(start,goal,cost);
    // afficher_liste_chemins(chemins);
    if(chemins.size()==0){
        std::cout<<"pas de chemin de longueur "<<cost<<std::endl;
        throw std::runtime_error("pas de chemin ? :(");
    }
    Node* racine= make_nodes(chemins);
    MDD nouveau(cost,racine);
    return nouveau;
}

void Graph::new_agent(const unsigned int x, const unsigned int y, const int id) {
    if (x >= width_ || y >= height_) throw std::out_of_range("Tried to place outside the graph");
    grille_[y][x] = std::make_unique<Agent>(id);
}
std::pair<int,int> Graph::pos_clicked(sf::Window & w){
    sf::Vector2u size = w.getSize();
    auto [width, height] = size;
    auto [m_x, m_y] = sf::Mouse::getPosition(w);
    int x=m_x/(width/width_);
    int y=m_y/(height/height_);
    if (x>width_-1){
        x=width_-1;
    }
    if (y>height_-1){
        y=height_-1;
    }
    if (x<0){
        x=0;    
    }
    if (y<0){
        y=0;
    }
    return std::pair<int,int>(x,y);
}

void Graph::make_lab(){
    std::srand(std::time({}));
    for (int i = 0; i < width_; i+=2)
    {
        for (int j = 0; j < height_; j+=2)
        {
            Graph::new_wall(Position(i,j));
            std::vector<Position> neighboors;
                if ((i +1 < width_) && (grille_[j][i+1]) == nullptr) {
                    neighboors.push_back(Position(i+1,j));
                }
                if ((j +1 < height_) && (grille_[j+1][i]) == nullptr) {
                    neighboors.push_back(Position(i,j+1));
                }
                if ((i > 0) && (grille_[j][j-1]) == nullptr) {
                    neighboors.push_back(Position(i-1,j));
                }
                if ((j > 0) && (grille_[j-1][i]) == nullptr) {
                    neighboors.push_back(Position(i,j-1));
                }
            int r=std::rand()%neighboors.size();
            Graph::new_wall(neighboors[r]);
        }
    }
    
}

StartsAndGoals Graph::draw(){
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Grille de MAPF");
    window.setFramerateLimit(framerate);
    //sf::Clock clock; // starts the clock
    StartsAndGoals ret=StartsAndGoals (10,std::pair<Position*,Position*>(nullptr,nullptr));
    sf::Vector2u size = window.getSize();
    auto [width, height] = size;

    while (window.isOpen())
    {
        auto c = sf::Color(0,0,0); 

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
            auto [x,y]=Graph::pos_clicked(window);
            Graph::new_wall(Position(x,y));
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            {
                auto [x,y]=Graph::pos_clicked(window);
                Graph::set_empty(Position(x,y));
                for(int i=0;i<10;++i){
                    if(ret[i].second!=nullptr and Position(x,y)==*ret[i].second){
                        delete ret[i].second;
                        ret[i].second=nullptr;
                    }
                    if(ret[i].first!=nullptr and Position(x,y)==*ret[i].first){
                        delete ret[i].first;
                        ret[i].first=nullptr;
                    }
                }
                
            }
            int num_pressed=-1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)){
                num_pressed=1;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)){
                num_pressed=2;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)){
                num_pressed=3;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)){
                num_pressed=4;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)){
                num_pressed=5;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)){
                num_pressed=6;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)){
                num_pressed=7;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)){
                num_pressed=8;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)){
                num_pressed=9;
            }
            if (num_pressed!=-1)
            {
            auto [x,y]=Graph::pos_clicked(window);
            if(ret[num_pressed].first==nullptr){
                    ret[num_pressed].first=new Position(x,y);
                    //Graph::new_agent(*ret[num_pressed].first,num_pressed); //got rid of agents because cange of design
                }
            else if(ret[num_pressed].second==nullptr){
                if(Position(x,y)!=*ret[num_pressed].first)
                ret[num_pressed].second=new Position(x,y);
            }
            }
        }

        window.clear(c);

        for(int i=0;i<get_width();i++){
            for(int j=0;j<get_height();j++){
                sf::RectangleShape box({(float)(width/get_width()-1), (float)(height/get_height()-1)});
                if(is_empty(Position(i,j))){
                    box.setFillColor(sf::Color(200,200,200)); 
                    for(int a=0; a<10;a++){
                        if(ret[a].first!=nullptr and Position(i,j)==*ret[a].first)
                            box.setFillColor(sf::Color(0,25*a,200)); 
                        if(ret[a].second!=nullptr and Position(i,j)==*ret[a].second)
                            box.setFillColor(sf::Color(25*a,200,0)); 
                    }                    
                }
                else if(is_agent(Position(i,j)))
                        box.setFillColor(sf::Color(0,0,200));
                else 
                    box.setFillColor(sf::Color(0,0,0)); 
                box.setPosition({(float)(1+i*(width/width_)),(float)(1+j*(height/height_))});
                window.draw(box);
            }
        }
        

        window.display();
    }
    return ret;
}

void Graph::show_path(
    const Position &start,
    const Position &goal,
    std::function<unsigned int (const Position&)> h
    ){
    Graph::new_agent(start,1);
    auto vect =Graph::a_star(start,goal,h);

    /*auto chemins=chemins_taille_n(start,goal,vect.size());
    afficher_liste_chemins(chemins);*/

    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Grille de MAPF");
    window.setFramerateLimit(framerate);
    sf::Clock clock; // starts the clock
    int a=0;
    sf::Time t=sf::seconds(0.2f);

    sf::Vector2u size = window.getSize();
    auto [width, height] = size;

    while (window.isOpen())
    {
        auto c = sf::Color(0,0,0); 

        

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

        }
        if(clock.getElapsedTime()> t and a<vect.size()){
            clock.restart();
            ++a;
            t=t*0.98f;
        }

        window.clear(c);

        for(int i=0;i<width_;i++){
            for(int j=0;j<height_;j++){
                sf::RectangleShape box({(float)(width/width_-1), (float)(height/height_-1)});
                if(is_empty(Position(i,j))){
                    auto beg=vect.end();
                    for(int e = 0; e<a;++e){
                        beg--;
                    }
                    if(Position(i,j)==goal){
                        box.setFillColor(sf::Color(200,200,0)); 
                    } 
                    else if(std::find(beg,vect.end(),Position(i,j))==vect.end()){
                        box.setFillColor(sf::Color(200,200,200)); 

                    }
                    else box.setFillColor(sf::Color(0,200,0)); 
                }
                else if(is_agent(Position(i,j))){
                    box.setFillColor(sf::Color(0,0,200)); 
                }
                else 
                    box.setFillColor(sf::Color(0,0,0)); 
                box.setPosition({(float)(1+i*(width/width_)),(float)(1+j*(height/height_))});
                window.draw(box);
            }
        }
        

        window.display();
    }
    Graph::set_empty(start);
}

void Graph::show_thoughts(
    const Position &start,
    const Position &goal,
    std::function<unsigned int (const Position&)> h
    ){
    Graph::new_agent(start,1);

    std::map<Position,Position> cameFrom;
    std::vector<std::vector<unsigned int>> gScore(height_, std::vector<unsigned int>(width_, std::numeric_limits<unsigned int>::max()));
    std::vector<std::vector<unsigned int>> fScore(height_, std::vector<unsigned int>(width_, std::numeric_limits<unsigned int>::max()));
    gScore[start.get_y()][start.get_x()] = 0;
    fScore[start.get_y()][start.get_x()] = h(start);

    // Init priority queue
    PriorityQueue<Position> open_nodes;
    std::vector<Position> seen;
    open_nodes.push_back(start,fScore[start.get_y()][start.get_x()]);
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Grille de MAPF");
    window.setFramerateLimit(framerate);
    sf::Clock clock; // starts the clock
    int a=0;
    sf::Time t=sf::seconds(0.2f);

    sf::Vector2u size = window.getSize();
    auto [width, height] = size;
    bool trouve = false;
    std::vector<Position> vect;
    while (window.isOpen())
    {
        auto c = sf::Color(0,0,0); 
        
        for(int i=0;i<etapes_par_frames;++i){
            if (!open_nodes.is_empty() and !trouve) {
            Position current = open_nodes.top();
            seen.push_back(current);
            if(current==goal){
                trouve=true;
                vect.push_back(current);
                while (current != start) {
                    current = cameFrom[current]; //si c'est current, cause un core dump, si current est remplacé par test, pas de problème.
                    vect.push_back(current);
                }
            }
            else{
                open_nodes.pop_front();

                std::vector<Position> neighboors;
                if ((current.get_x() +1 < width_) && (grille_[current.get_y()][current.get_x()+1]) == nullptr) {
                    neighboors.push_back(Position(current.get_x()+1,current.get_y()));
                }
                if ((current.get_y() +1 < height_) && (grille_[current.get_y()+1][current.get_x()]) == nullptr) {
                    neighboors.push_back(Position(current.get_x(),current.get_y()+1));
                }
                if ((current.get_x() > 0) && (grille_[current.get_y()][current.get_x()-1]) == nullptr) {
                    neighboors.push_back(Position(current.get_x()-1,current.get_y()));
                }
                if ((current.get_y() > 0) && (grille_[current.get_y()-1][current.get_x()]) == nullptr) {
                    neighboors.push_back(Position(current.get_x(),current.get_y()-1));
                }
                for (Position &npos : neighboors) {
                    unsigned int new_score = gScore[current.get_y()][current.get_x()] +1;
                    if (new_score < gScore[npos.get_y()][npos.get_x()]) {
                        cameFrom[npos] = current;
                        gScore[npos.get_y()][npos.get_x()] = new_score;
                        fScore[npos.get_y()][npos.get_x()] = new_score + h(npos);
                        
                        if (!open_nodes.find_if_in(npos)) {
                            open_nodes.push_back(npos,fScore[npos.get_y()][npos.get_x()]);
                        }
                    }
                }
            }
            
        }
        }
            
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

        }
        /*if(clock.getElapsedTime()> t and a<vect.size()){
            clock.restart();
            ++a;
            t=t*0.98f;
        }*/

        window.clear(c);
        int max=fScore[start.get_y()][start.get_x()];
        for(int i=0;i<width_;i++){
            for(int j=0;j<height_;j++){
                sf::RectangleShape box({(float)(width/width_-1), (float)(height/height_-1)});
                if(is_empty(Position(i,j))){
                    auto beg=vect.end();
                    for(int e = 0; e<a;++e){
                        beg--;
                    }
                    if(Position(i,j)==goal){
                        box.setFillColor(sf::Color(200,200,0)); //goal
                    }
                    else if(std::find(vect.begin(),vect.end(),Position(i,j))!=vect.end()){//case du chemin
                        box.setFillColor(sf::Color(0,200,0)); 
                    }
                    /*else if(trouve and std::find(beg,vect.end(),Position(i,j))!=vect.end()){//case du chemin
                        box.setFillColor(sf::Color(0,200,0)); 

                    }*/
                    else if(std::find(seen.begin(),seen.end(),Position(i,j))!=seen.end()){//case vue
                        if(fScore[j][i]>max){
                            max=fScore[j][i];
                        }
                        box.setFillColor(sf::Color((255*gScore[j][i]/max),150,150));
                    }
                    
                    else box.setFillColor(sf::Color(200,200,200)); //case vide
                }
                else if(is_agent(Position(i,j))){
                    box.setFillColor(sf::Color(0,0,200)); 
                }
                else 
                    box.setFillColor(sf::Color(0,0,0)); 
                box.setPosition({(float)(1+i*(width/width_)),(float)(1+j*(height/height_))});
                window.draw(box);
            }
        }
        

        window.display();
    }
    Graph::set_empty(start);
}

std::vector<KdimPosition> Graph::mapf_icst(StartsAndGoals & temp, const conflicts &c){
    //algorythme de mapf
    std::vector<unsigned int> cost_list;
    StartsAndGoals S;
    for (int i = 0; i < temp.size(); i++)//on crée la liste avec seulement les agents qui existent
    {    
        if(temp[i].first!=nullptr){//si l'agent existe
            S.push_back(temp[i]);
        }
        
    }
    for (int i = 0; i < S.size(); i++)//pour chaque paire agent/destination
    {    
        unsigned int cout= Graph::longeur_a_star(*(S[i].first),*(S[i].second),(S[i].second)->dist_taxicab_to())-1;
        if(cout==0) {
            throw std::runtime_error("single agent pathfinding isn't solvable...... what do you want me to do ?");
        }
        cost_list.push_back(cout);
    }
    ICST I(cost_list);
    bool continuer;
    sf::Clock clock; // starts the clock
    sf::Time t=sf::seconds(300);
    MDD* final;
    while (continuer)
    {
        if(clock.getElapsedTime()> t){
        continuer=false;
        throw std::runtime_error("took too long");
        }
        else{
            continuer=false;//on suppose que ce sera la dernière étape
            auto tab=I.next();
            std::vector<MDD*> MDD_List;
            std::cout<<"Nouvelle itération : "<<std::endl<<"("<<tab[0];
            for (int j = 1; j < tab.size(); j++)//oui, retirer cette boucle for empèche le programe de fonctionner. oui . ?!!?!??.
            {
                std::cout<<", "<<tab[j];
            }
            std::cout<<")"<<std::endl;
            for (int i = 0; i < tab.size(); i++)
            {
                MDD* M= new MDD(MDD::fabric_new(*(S[i].first),*(S[i].second),tab[i],*this));
                MDD_List.push_back(M);
            }
            // std::cout<<"avant prunning"<<std::endl;
            // for (int i = 1; i < tab.size(); i=i+2)
            // {
            //     //std::cout<<MDD_List[0].get_dim()<<std::endl;
            //     MDD::cross_prunning(*MDD_List[i-1],*MDD_List[i],c);
            // }
            // std::cout<<"après prunning"<<std::endl;
            final=new MDD(*MDD_List[0]);//works so MDD copy only breaks above
            //std::cout<<"ici"<<std::endl;
            for (int i = 1; i < tab.size(); i++)
            {
                try
                {
                    MDD M(MDD::fusion(*final,*MDD_List[i],c));
                    // std::cout<<"taille de M :"<<M.get_dim()<<std::endl;
                    *final=M;
                    // std::cout<<"là"<<std::endl;
                }
                catch(const std::runtime_error& e)//la fusion à échoué
                {
                    //std::cout<<e.what()<<std::endl;
                    continuer=true;//il faut passer à l'étape suivante
                    break;
                }                
            }
            for (int i = 0; i < MDD_List.size(); i++)
            {
                delete MDD_List[i];
            }            
        }
    }
    std::vector<KdimPosition> resultat=(*final).output();
    //std::cout<<resultat.size()<<std::endl;
    //auto test =resultat[0].get_dim_k();
    /*for (int  i = 0; i+1 < resultat[0].get_dim_k(); i++)
    {
        std::cout<<resultat[0].nth_pos(i)<<"|"<<std::endl;
    }*/
    return resultat;
}

void Graph::show_mapf_solution( const std::vector<KdimPosition> &vect){
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Grille de MAPF");
    window.setFramerateLimit(framerate);
    sf::Clock clock; // starts the clock
    int step=0;
    sf::Time t=sf::seconds(1.0f);
    sf::Vector2u size = window.getSize();
    auto [width, height] = size;

    while (window.isOpen())
    {
        auto c = sf::Color(0,0,0); 
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

        }
        if(clock.getElapsedTime()> t and step<vect.size()-1){
            clock.restart();
            // for (int  i = 0; i < vect[0].get_dim_k(); i++)
            // {
            //     std::cout<<vect[step].nth_pos(i)<<"|";
            // }
            // std::cout<<std::endl;
            ++step;
            t=t*0.98f;
        }

        window.clear(c);
        //sf::Font font("../src/ARIAL.TTF");
        for(int i=0;i<width_;i++){
            for(int j=0;j<height_;j++){
                sf::RectangleShape box({(float)(width/width_-1), (float)(height/height_-1)});
                //sf::Text text(font);
                // set the character size
                //text.setCharacterSize(50);
                //text.setFillColor(sf::Color::Red);
                // text.setStyle(sf::Text::Bold | sf::Text::Underlined);
                box.setFillColor(sf::Color(200,200,200)); //case vide
                if(is_empty(Position(i,j))){
                    for(int x=0;x<vect[0].get_dim_k();++x){//pour chaque agent
                        if(Position(i,j)==vect[vect.size()-1].nth_pos(x)){//si la case est une case d'arrivée
                            box.setFillColor(sf::Color(200,200,0));
                            //text.setString(std::to_string(x));
                            //text.setPosition({(float)(1+i*(width/width_)),(float)(1+j*(height/height_))});
                        } 
                        else if(Position(i,j)==vect[step].nth_pos(x)){//si la case est celle que l'agent visite à cette étape
                            //text.setString(std::to_string(x));
                            //text.setPosition({(float)(1+i*(width/width_)),(float)(1+j*(height/height_))});
                            box.setFillColor(sf::Color(0,200,0));
                        }
                    }
                }
                else {
                    if(is_wall(Position(i,j))) box.setFillColor(sf::Color(0,0,0)); 
                }
                    
                box.setPosition({(float)(1+i*(width/width_)),(float)(1+j*(height/height_))});
                window.draw(box);
                //window.draw(text);
            }
        }
        window.display();
    }
}

std::vector<Position> Graph::a_star(
    const Position &start,
    const Position &goal,
    std::function<unsigned int (const Position&)> h
    ) const {
    
    // Preceding node on the shortest path
    std::map<Position,Position> cameFrom;
    // Current cost
    std::vector<std::vector<unsigned int>> gScore(height_, std::vector<unsigned int>(width_, std::numeric_limits<unsigned int>::max()));
    // Cheapest path that goes through
    std::vector<std::vector<unsigned int>> fScore(height_, std::vector<unsigned int>(width_, std::numeric_limits<unsigned int>::max()));
    gScore[start.get_y()][start.get_x()] = 0;
    fScore[start.get_y()][start.get_x()] = h(start);

    // Init priority queue
    PriorityQueue<Position> open_nodes;
    open_nodes.push_back(start,fScore[start.get_y()][start.get_x()]);
    
    while (!open_nodes.is_empty()) {
        Position current = open_nodes.top();

        if (current == goal) {
            std::vector<Position> path;
            path.push_back(current);
            while (current != start) {
                current = cameFrom[current];
                path.push_back(current);
            }
            return path;
        }
        open_nodes.pop_front();

        std::vector<Position> neighboors;
        if ((current.get_x() +1 < width_) && (grille_[current.get_y()][current.get_x()+1]) == nullptr) {
            neighboors.push_back(Position(current.get_x()+1,current.get_y()));
        }
        if ((current.get_y() +1 < height_) && (grille_[current.get_y()+1][current.get_x()]) == nullptr) {
            neighboors.push_back(Position(current.get_x(),current.get_y()+1));
        }
        if ((current.get_x() > 0) && (grille_[current.get_y()][current.get_x()-1]) == nullptr) {
            neighboors.push_back(Position(current.get_x()-1,current.get_y()));
        }
        if ((current.get_y() > 0) && (grille_[current.get_y()-1][current.get_x()]) == nullptr) {
            neighboors.push_back(Position(current.get_x(),current.get_y()-1));
        }
        for (Position &npos : neighboors) {
            unsigned int new_score = gScore[current.get_y()][current.get_x()] +1;
            if (new_score < gScore[npos.get_y()][npos.get_x()]) {
                cameFrom[npos] = current;
                gScore[npos.get_y()][npos.get_x()] = new_score;
                fScore[npos.get_y()][npos.get_x()] = new_score + h(npos);
                
                if (!open_nodes.find_if_in(npos)) open_nodes.push_back(npos, fScore[npos.get_y()][npos.get_x()]);
            }
        }
    }
    return std::vector<Position>();
}

unsigned int Graph::longeur_a_star(const Position &start, const Position &goal, std::function<unsigned int (const Position&)> h) const{
    return a_star(start,goal,h).size();
}