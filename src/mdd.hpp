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
    Position start_;
    // Position goal_;
    unsigned int cost_;
    std::vector<std::map<Position,Node>> diagram_;
public:
    // FIXME
    // private constructor and public fabric instead of this
    MDD(const Position &start, const Position &goal, const unsigned int cost, const unsigned int g_width, const unsigned int g_height);
    MDD(const MDD&) noexcept; //MUST
    MDD(MDD&&) noexcept = default;

    // Prune the two MDD to keep only compatible paths in both MDDs.
    // Return whether there is still at least one path for each MDD
    static bool cross_prunning(MDD &a, MDD &b) noexcept;
    // Perform the search of a k-cross-MDD.
    // Return a path for that MDD, solving MAPF
    static std::vector<std::vector<Position>> kMDD(std::vector<MDD> MDDs) noexcept;
};
