#include "position.hpp"

Agent::Agent(int id) noexcept : Cell(), id_(id) {}

int Agent::get_id() const noexcept {
    return id_;
}

Position::Position() noexcept : x_(-1), y_(-1) {}

Position::Position(unsigned int x, unsigned int y) noexcept : x_(x), y_(y) {}

// void Position::set_x(unsigned int x) noexcept {
//     x_ = x;
// }

// void Position::set_y(unsigned int y) noexcept {
//     y_ = y;
// }

unsigned int Position::get_x() const noexcept {
    return x_;
}

unsigned int Position::get_y() const noexcept {
    return y_;
}

bool Position::operator==(const Position &pos) const noexcept {
    return x_ == pos.x_ && y_ == pos.y_;
}

bool Position::operator!=(const Position &pos) const noexcept {
    return !operator==(pos);
}

bool Position::operator<(const Position &pos) const noexcept {
    return x_ < pos.x_ || (x_ == pos.x_ && y_ < pos.y_);
}

unsigned int square(unsigned int x) noexcept {
    return x*x;
}

unsigned int Position::dist_eucl(const Position &a, const Position &b) noexcept {
    return a.dist_eucl(b);
}

unsigned int Position::dist_eucl(const Position &pos) const noexcept {
    unsigned int dx(x_ > pos.x_ ? x_ - pos.x_ : pos.x_ - x_);
    unsigned int dy(y_ > pos.y_ ? y_ - pos.y_ : pos.y_ - y_);
    return square(dx) + square(dy);
}

std::function<unsigned int (const Position&)> Position::dist_eucl_to() const noexcept {
    return [this](const Position &pos){return this->dist_eucl(pos);};
}

unsigned int Position::dist_taxicab(const Position &a, const Position &b) noexcept {
    return a.dist_taxicab(b);
}

unsigned int Position::dist_taxicab(const Position &pos) const noexcept {
    unsigned int dx(x_ > pos.x_ ? x_ - pos.x_ : pos.x_ - x_);
    unsigned int dy(y_ > pos.y_ ? y_ - pos.y_ : pos.y_ - y_);
    return dx + dy;
}

std::function<unsigned int (const Position&)> Position::dist_taxicab_to() const noexcept {
    return [this](const Position &pos){return this->dist_taxicab(pos);};
}

std::ostream& operator<<(std::ostream &stream, const Position &pos) {
    stream << "(" << pos.x_ << "," << pos.y_ << ")";
    return stream;
}