#pragma once

#include <vector>
#include <map>
#include "position.hpp"

struct Node {
    bool wrong_path;
    std::vector<Position> children;
};

class MDD {
private:
    // unsigned int agent;
    unsigned int cost_;
    Position start_;
    // Position goal_;
    std::vector<std::map<Position,Node>> diagram_;
public:
    MDD(const Position &start, const Position &goal, const unsigned int cost) noexcept;
    MDD(const MDD&) noexcept; //MUST
    MDD(MDD&&) noexcept = default;
};
