#pragma once

#include <vector>
#include <set>
#include "position.hpp"
#include "conflicts.hpp"
#include <queue>
#include <memory>

struct Node {
    KdimPosition pos;
    //std::set<Node*> children;
    std::set<Node*> children;
    bool valid_path;
};

class Graph;

Node new_node(const KdimPosition&) noexcept;

std::set<Node*> flag_for_deletion(Node *);

// void print_queue (std::queue<Node*> & file);

void delete_flaged(std::set<Node*> &);

void delete_node(Node *);

Node* copy_node(const Node &);

std::vector<Position> combiner(std::vector<Position> & a,std::vector<Position> & b, const conflicts &c);

bool check(std::vector<Position> &FromA, std::vector<Position> &FromB,std::vector<Position> &ToA,std::vector<Position> ToB, const conflicts &c);

class MDD {
private:
    unsigned int cost_;
    Node* root_;
    MDD(const unsigned int cost, Node* const root);

public:
    static Node* make_nodes(std::vector<std::vector<Position>>& chemins);
    static MDD fabric_new(const Position &start, const Position &goal, const unsigned int cost, const Graph & g);
    MDD(const MDD&); //MUST
    MDD(MDD&&) noexcept = default;
    ~MDD();
    bool is_empty() const noexcept;
    int get_cost() const;
    int get_dim() const;
    Node* get_root() const;//somehow breaks EVERYTHING, like c++ itself
    // Prune the two MDD to keep only compatible paths in both MDDs.
    // Return whether there is still at least one path for each MDD
    static void cross_prunning(MDD &a, MDD &b,const conflicts &c) noexcept;
    static void cross_prunning_rec(Node &a, Node &b,const conflicts &c) noexcept;
    static MDD fusion(const MDD &a,const MDD &b,const conflicts &c) ;
    static std::pair<bool,Node> fusion_rec(Node &a, Node &b,const conflicts &c, int n) noexcept;
    std::vector<KdimPosition> output();
    // Perform the search of a k-cross-MDD.
    // Return a path for that MDD, solving MAPF
    // static std::vector<std::vector<Position>> kMDD(std::vector<MDD> MDDs) noexcept;
    void operator=(const MDD & M);
};