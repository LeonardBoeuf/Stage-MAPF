#include "mdd.hpp"
#include <exception>
#include <algorithm>

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

std::vector<KdimPosition> neighboors(const KdimPosition &pos, const unsigned int width, const unsigned int height) {
    // TODO
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