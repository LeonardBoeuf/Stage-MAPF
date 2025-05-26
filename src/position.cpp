#include "position.hpp"

Agent::Agent(int id) noexcept : Cell(), id_(id) {}

int Cell::get_id() const noexcept {
    return -1;
}

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


KdimPosition::KdimPosition(const std::vector<Position> pos) : pos_(pos) {}

KdimPosition::KdimPosition(const KdimPosition &kpos) : pos_(kpos.pos_) {}

KdimPosition& KdimPosition::operator=(const KdimPosition &kpos) {
    if (*this != kpos) {
        this->pos_.empty();
        for (int k(0); k<kpos.pos_.size(); ++k) this->pos_.push_back(kpos.pos_[k]);
    }
    return *this;
}

unsigned int KdimPosition::get_dim_k() const noexcept {
    return pos_.size();
}

const Position& KdimPosition::nth_pos(const unsigned int n) const throw() {
    return pos_.at(n);
}

bool KdimPosition::operator==(const KdimPosition &kpos) const noexcept {
    if (pos_.size() != kpos.pos_.size()) return false;
    for (int k(0); k<pos_.size(); ++k) if (pos_[k] != kpos.pos_[k]) return false;
    return true;
}

bool KdimPosition::operator!=(const KdimPosition &kpos) const noexcept {
    return !operator==(kpos);
}

KdimPosition KdimPosition::merge(const KdimPosition &a, const KdimPosition &b) noexcept {
    unsigned int a_size(a.pos_.size());
    unsigned int b_size(b.pos_.size());
    std::vector<Position> pos(a_size + b_size);

    for (int k(0); k<a_size; ++k) pos.push_back(a.pos_[k]);
    for (int k(0); k<b_size; ++k) pos.push_back(b.pos_[k]);

    return KdimPosition(pos);
}
