#pragma once
#include <iostream>
#include <functional>
#include <vector>

class Cell {
public:
    virtual ~Cell() = default;
    virtual int get_id() const noexcept;
};

class Wall : public Cell {};
class Agent : public Cell {
private:
    int id_;
public:
    Agent(int id) noexcept;

    int get_id() const noexcept override;
};

class Position {
private:
    unsigned int x_;
    unsigned int y_;
public:
    // For map
    Position() noexcept;
    Position(unsigned int x, unsigned int y) noexcept;
    // void set_x(unsigned int x) noexcept;
    // void set_y(unsigned int y) noexcept;
    unsigned int get_x() const noexcept;
    unsigned int get_y() const noexcept;

    bool operator==(const Position &pos) const noexcept;
    bool operator!=(const Position &pos) const noexcept;
    friend std::ostream& operator<<(std::ostream &stream, const Position &pos);

    // For map
    bool operator<(const Position &pos) const noexcept;

    static unsigned int dist_eucl(const Position &a, const Position &b) noexcept;
    unsigned int dist_eucl(const Position &pos) const noexcept;
    std::function<unsigned int (const Position&)> dist_eucl_to() const noexcept;

    static unsigned int dist_taxicab(const Position &a, const Position &b) noexcept;
    unsigned int dist_taxicab(const Position &pos) const noexcept;
    std::function<unsigned int (const Position&)> dist_taxicab_to() const noexcept;
};

class KdimPosition {
private:
    std::vector<Position> pos_;
public:
    KdimPosition(const std::vector<Position> pos) noexcept;
    KdimPosition(const Position pos) noexcept;
    KdimPosition(const KdimPosition&) = default;
    KdimPosition(KdimPosition&&) = default;
    KdimPosition& operator=(const KdimPosition& kpos) noexcept;

    unsigned int get_dim_k() const noexcept;
    const Position& nth_pos(const unsigned int n) const throw();

    bool operator==(const KdimPosition& kpos) const noexcept;
    bool operator!=(const KdimPosition& kpos) const noexcept;

    static KdimPosition merge(const KdimPosition &a, const KdimPosition &b) noexcept;
};