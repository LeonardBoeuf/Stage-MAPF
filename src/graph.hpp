#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <exception>
#include <ostream>

class Cell {
public:
    virtual ~Cell() = default;
};

class Wall : public Cell {};
class Agent : public Cell {
private:
    int id_;
public:
    Agent(int id) noexcept;

    int get_id() const noexcept;
};

class Position {
private:
    unsigned int x_;
    unsigned int y_;
public:
    // For map
    Position() noexcept;
    Position(unsigned int x, unsigned int y) noexcept;
    // void set_x(unsigned int x) noexcept;
    // void set_y(unsigned int y) noexcept;
    unsigned int get_x() const noexcept;
    unsigned int get_y() const noexcept;

    bool operator==(const Position &pos) const noexcept;
    bool operator!=(const Position &pos) const noexcept;
    friend std::ostream& operator<<(std::ostream &stream, const Position &pos);

    // For map
    bool operator<(const Position &pos) const noexcept;

    static unsigned int dist_eucl(const Position &a, const Position &b) noexcept;
    unsigned int dist_eucl(const Position &pos) const noexcept;
};



using Grille = std::vector<std::vector<std::unique_ptr<Cell>>>;

class GraphException : public std::exception {

};

class Graph {
private:
    unsigned int width_;
    unsigned int height_;
    std::vector<std::vector<std::unique_ptr<Cell>>> grille_;
public:
    Graph(unsigned int width, unsigned int height);
    Graph(const Graph&) = delete;
    Graph(Graph&&) = default;
    
    bool is_empty(const Position &pos) const;
    bool is_wall(const Position &pos) const;
    bool is_agent(const Position &pos) const;
    // int get_agent_id(const Position &pos) const;
    // std::pair<int,int> get_agent_pos(const int id) const;
    unsigned int get_width() const noexcept;
    unsigned int get_height() const noexcept;
    
    void set_empty(const Position &pos);
    void set_empty(const unsigned int x, const unsigned int y);
    void new_wall(const Position &pos);
    void new_wall(const unsigned int x, const unsigned int y);
    void new_agent(const Position &pos, const int id);
    void new_agent(const unsigned int x, const unsigned int y, const int id);
    // void move_agent(const Position &from, const Position &to);

    void run();
    void run(const Position &start, const Position &goal, std::function<unsigned int (const Position&)> h);
    
    std::vector<Position> a_star(const Position &start, const Position &goal, std::function<unsigned int (const Position&)> h) const;
};