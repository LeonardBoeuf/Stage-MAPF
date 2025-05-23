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