#include "icst.hpp"
#include <algorithm>

ICST::ICST(const std::vector<unsigned int> &cost_list) noexcept : nodes_(1, cost_list) {}

Node ICST::next() noexcept {
    Node next(nodes_[0]);
    nodes_.erase(nodes_.begin());

    for (int i_cost(0); i_cost<next.size(); ++i_cost) {
        Node new_node(next);
        new_node[i_cost]++;
        if (std::find(nodes_.begin(),nodes_.end(),new_node) == nodes_.end())
            nodes_.push_back(new_node);
    }

    return next;
}