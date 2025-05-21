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
#include "mdd.hpp"

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
    // return grille_[pos.get_y()][pos.get_x()] == nullptr;
    return grille_.at(pos.get_y()).at(pos.get_x()) == nullptr;
}

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
                    Graph::new_agent(*ret[num_pressed].first,num_pressed);
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


std::vector<std::vector<Position>> Graph::icst(const std::vector<Position> &starts, const std::vector<Position> &goals, const float maxComputeTime) const {
    // Check validity of input
    unsigned int k = starts.size();
    if (k == 0 || goals.size() != k) return std::vector<std::vector<Position>>(); // throw exception instead ?

    // A_star all agents
    std::vector<std::vector<Position>> a_star_paths(k);
    std::vector<unsigned int> a_star_costs(k);
    for(int i(0); i<k; ++i) {
        a_star_paths.push_back(this->a_star(starts[i],goals[i],goals[i].dist_taxicab_to()));
        if (a_star_paths[i].size() <= 0) return std::vector<std::vector<Position>>(); // No solution
        a_star_costs.push_back(a_star_costs.size());
    }

    // Init ICST (Increasing Cost Search Tree)
    ICST icst(a_star_costs);
    // Init MDDs matrix
    std::vector<std::vector<MDD>> mdds(k,std::vector<MDD>());
    sf::Time maxComputeTime_(sf::seconds(maxComputeTime));
    sf::Clock timer;
    while (timer.getElapsedTime() < maxComputeTime_) {
        std::vector<unsigned int> icst_node(icst.next());

        // Init or create MDDs with desired cost
        if (mdds[0].size() == 0) {
            for (int i(0); i<k; ++i) {
                //mdds[i].push_back(MDD(starts[i],goals[i],s_star_costs[i]));
            }
        } else {
            for (int i(0); i<k; ++i) {
                unsigned int delta (icst_node[i] - a_star_costs[i]);
                if (mdds[i].size() == delta) { // Cannot be <
                    //mdds[i].push_back(mdds[i][delta-1].create_increased_cost());
                }
            }
        }

        // Pairwise prunning
        for (int i(0); i<k; ++i) {
            for (int j(i+1); j<k; ++j) {
                //  if (!MDD::cross_prunning(mdds[i][icst_node[i] - a_star_costs[i]],mdds[j][icst_node[j] - a_star_costs[j]])) {
                //      return std::vector<std::vector<Position>>(); // No solution
                // }
            }
        }

        //  Searching k-MDD
        //  std::vector<std::vector<Position>> kMDD();
        //  if (kMDD.size() > 0) {
        //      return kMDD;
        //  }
    }
}