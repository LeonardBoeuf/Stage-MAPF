#include "mdd.hpp"
#include <exception>
#include <algorithm>

Node new_node(std::vector<Position> pos) noexcept {
    Node n;
    n.children = std::set<std::vector<Position>>();
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

MDD::MDD(const unsigned int cost, const std::vector<Position> &root) : cost_(cost), root_(root) {
}

MDD MDD::fabric_new(
    const std::vector<Position> &start,
    const std::vector<Position> &goal,
    const unsigned int cost,
    const unsigned int g_width,
    const unsigned int g_height
) {
    if (cost == 0) throw std::invalid_argument("Cannot handle 0-cost MDDs");
}