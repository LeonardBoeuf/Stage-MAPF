#pragma once

#include <vector>
#include <set>
#include "position.hpp"
#include "conflicts.hpp"

struct Node {
    KdimPosition pos;
    std::set<Node*> children;
    bool valid_path;
};

Node new_node(const KdimPosition&) noexcept;

std::vector<Position> combiner(std::vector<Position> & a,std::vector<Position> & b, const conflicts &c);

bool check(std::vector<Position> &FromA, std::vector<Position> &FromB,std::vector<Position> &ToA,std::vector<Position> ToB, const conflicts &c);

class MDD {
private:
    unsigned int cost_;
    Node* root_;
    MDD(const unsigned int cost, Node* const root);

public:
    static MDD fabric_new(const KdimPosition &start, const KdimPosition &goal, const unsigned int cost, const unsigned int g_width, const unsigned int g_height);
    MDD(const MDD&) noexcept = default; //MUST
    MDD(MDD&&) noexcept = default;

    bool is_empty() const noexcept;

    // Prune the two MDD to keep only compatible paths in both MDDs.
    // Return whether there is still at least one path for each MDD
    static MDD cross_prunning(MDD &a, MDD &b,const conflicts &c) noexcept;
    // Perform the search of a k-cross-MDD.
    // Return a path for that MDD, solving MAPF
    // static std::vector<std::vector<Position>> kMDD(std::vector<MDD> MDDs) noexcept;
};
