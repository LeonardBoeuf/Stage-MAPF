#include "graph.hpp"
#include <queue>
#include <map>
#include <algorithm>
#include <limits>
#include <SFML/Graphics.hpp>

Agent::Agent(int id) noexcept : Cell(), id_(id) {}

int Agent::get_id() const noexcept {
    return id_;
}


Position::Position() noexcept : x_(-1), y_(-1) {}

Position::Position(unsigned int x, unsigned int y) noexcept : x_(x), y_(y) {}

// void Position::set_x(unsigned int x) noexcept {
//     x_ = x;
// }

// void Position::set_y(unsigned int y) noexcept {
//     y_ = y;
// }

unsigned int Position::get_x() const noexcept {
    return x_;
}

unsigned int Position::get_y() const noexcept {
    return y_;
}

bool Position::operator==(const Position &pos) const noexcept {
    return x_ == pos.x_ && y_ == pos.y_;
}

bool Position::operator!=(const Position &pos) const noexcept {
    return !operator==(pos);
}

bool Position::operator<(const Position &pos) const noexcept {
    return x_ < pos.x_ || (x_ == pos.x_ && y_ < pos.y_);
}

unsigned int square(unsigned int x) noexcept {
    return x*x;
}

unsigned int Position::dist_eucl(const Position &a, const Position &b) noexcept {
    return a.dist_eucl(b);
}

unsigned int Position::dist_eucl(const Position &pos) const noexcept {
    return square(x_ - pos.x_) + square(y_ - pos.y_);
}

std::ostream& operator<<(std::ostream &stream, const Position &pos) {
    stream << "(" << pos.x_ << "," << pos.y_ << ")";
    return stream;
}



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

void Graph::run(){
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Grille de MAPF");
    window.setFramerateLimit(144);
    //sf::Clock clock; // starts the clock

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

        window.clear(c);

        for(int i=0;i<get_width();i++){
            for(int j=0;j<get_height();j++){
                sf::RectangleShape box({(float)(width/get_width()-1), (float)(height/get_height()-1)});
                if(is_empty(Position(i,j)))
                    box.setFillColor(sf::Color(200,200,200)); 
                else if(is_agent(Position(i,j)))
                    box.setFillColor(sf::Color(0,0,200)); 
                else 
                    box.setFillColor(sf::Color(0,0,0)); 
                box.setPosition({(float)(1+i*width/get_width()),(float)(1+j*height/get_height())});
                window.draw(box);
            }
        }
        

        window.display();
    }
}

void Graph::run(
    const Position &start,
    const Position &goal,
    std::function<unsigned int (const Position&)> h
    ){
    Graph::new_agent(start,1);
    auto vect =Graph::a_star(start,goal,h);

    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Grille de MAPF");
    window.setFramerateLimit(900);
    sf::Clock clock; // starts the clock
    int a=0;

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
        if(clock.getElapsedTime()>sf::seconds(1.0f)and a<vect.size()){
            clock.restart();
            ++a;
        }

        window.clear(c);

        for(int i=0;i<get_width();i++){
            for(int j=0;j<get_height();j++){
                sf::RectangleShape box({(float)(width/get_width()-1), (float)(height/get_height()-1)});
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
                box.setPosition({(float)(1+i*width/get_width()),(float)(1+j*height/get_height())});
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
    // std::vector<std::vector<Position>> cameFrom(height_, std::vector<Position>(width_, Position(0,0)));
    std::map<Position,Position> cameFrom;
    // Current cost
    std::vector<std::vector<unsigned int>> gScore(height_, std::vector<unsigned int>(width_, std::numeric_limits<unsigned int>::max()));
    // Cheapest path that goes through
    std::vector<std::vector<unsigned int>> fScore(height_, std::vector<unsigned int>(width_, std::numeric_limits<unsigned int>::max()));
    gScore[start.get_y()][start.get_x()] = 0;
    fScore[start.get_y()][start.get_x()] = h(start);

    // Init priority queue
    auto cmp = [fScore](const Position &left, const Position &right) {
        return fScore[left.get_y()][left.get_x()] < fScore[right.get_y()][right.get_x()];
    };
    std::priority_queue<Position, std::vector<Position>, decltype(cmp)> open_nodes(cmp);
    std::vector<Position> pos_in_open;
    open_nodes.push(start);
    pos_in_open.push_back(start);
    while (!open_nodes.empty()) {
        Position current = open_nodes.top();
        // std::cout << "Next : " << current << std::endl;

        if (current == goal) {
            std::vector<Position> path;
            path.push_back(current);
            while (current != start) {
                current = cameFrom[current];
                path.push_back(current);
            }
            return path;
        }
        open_nodes.pop();
        pos_in_open.erase(std::find(pos_in_open.begin(),pos_in_open.end(),current));

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
            unsigned int new_score = gScore[current.get_y()][current.get_x()];
            if (new_score < gScore[npos.get_y()][npos.get_x()]) {
                // cameFrom[npos.get_y()][npos.get_x()] = current;
                cameFrom[npos] = current;
                gScore[npos.get_y()][npos.get_x()] = new_score;
                fScore[npos.get_y()][npos.get_x()] = new_score + h(npos);
                
                auto is_in_open = std::find(pos_in_open.begin(),pos_in_open.end(),npos);
                if (is_in_open == pos_in_open.end()) {
                    open_nodes.push(npos);
                    pos_in_open.push_back(npos);
                }
            }
        }
    }

    return std::vector<Position>();
}