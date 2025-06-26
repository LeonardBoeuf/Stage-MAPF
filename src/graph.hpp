#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <exception>

#include "mdd.hpp"
#include "icst.hpp"
#include "position.hpp"



using Grille = std::vector<std::vector<std::unique_ptr<Cell>>>;

class GraphException : public std::exception {

};

using StartsAndGoals = std::vector<std::pair<Position*,Position*>>;

void afficher_liste_chemins(const std::vector<std::vector<Position>> & c);

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
    // Position get_agent_pos(const int id) const;
    unsigned int get_width() const noexcept;
    unsigned int get_height() const noexcept;
    
    void set_empty(const Position &pos);
    void set_empty(const unsigned int x, const unsigned int y);
    void new_wall(const Position &pos);
    void new_wall(const unsigned int x, const unsigned int y);
    void new_agent(const Position &pos, const int id);
    void new_agent(const unsigned int x, const unsigned int y, const int id);
    // void move_agent(const Position &from, const Position &to);

    std::vector<std::vector<Position>> chemins_taille_n(const Position &start,const Position &goal,unsigned int n) const;

    std::pair<int,int> pos_clicked(sf::Window & w);

    void make_lab();
    StartsAndGoals draw();
    void show_path(const Position &start, const Position &goal, std::function<unsigned int (const Position&)> h);
    void show_thoughts(const Position &start, const Position &goal, std::function<unsigned int (const Position&)> h);
    
    std::vector<KdimPosition> mapf_icst(StartsAndGoals &, const conflicts &c);
    
    void show_mapf_solution(const std::vector<KdimPosition> &vect);

    std::vector<Position> a_star(const Position &start, const Position &goal, std::function<unsigned int (const Position&)> h) const;
    unsigned int longeur_a_star(const Position &start, const Position &goal, std::function<unsigned int (const Position&)> h) const;
    std::vector<std::vector<Position>> icst(const std::vector<Position> &starts, const std::vector<Position> &goals, const float maxComputeTime) const;
};