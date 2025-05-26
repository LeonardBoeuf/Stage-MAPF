#pragma once

#include <vector>
#include <set>
#include "position.hpp"
#include "conflicts.hpp"

struct Node {
    std::vector<Position> pos;
    std::set<Node*> children;
    bool valid_path;
};

Node new_node(std::vector<Position>) noexcept;

std::vector<Position> combiner(std::vector<Position> & a,std::vector<Position> & b, const conflicts &c={conflict_types::collision});

class MDD {
private:
    unsigned int cost_;
    Node* root_;
    MDD(const unsigned int cost, const std::vector<Position> &root);

public:
    static MDD fabric_new(const std::vector<Position> &start, const std::vector<Position> &goal, const unsigned int cost, const unsigned int g_width, const unsigned int g_height) throw();
    MDD(const MDD&) noexcept; //MUST
    MDD(MDD&&) noexcept = default;

    bool is_empty() const noexcept;

    // Prune the two MDD to keep only compatible paths in both MDDs.
    // Return whether there is still at least one path for each MDD
    static MDD cross_prunning(MDD &a, MDD &b,const conflicts &c={conflict_types::collision}) noexcept;
    // Perform the search of a k-cross-MDD.
    // Return a path for that MDD, solving MAPF
    // static std::vector<std::vector<Position>> kMDD(std::vector<MDD> MDDs) noexcept;
};
