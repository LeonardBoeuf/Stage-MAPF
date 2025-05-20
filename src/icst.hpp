#pragma once

#include <vector>

using Node = std::vector<unsigned int>;

class ICST {
private:
    std::vector<Node> nodes_;
public:
    ICST(const std::vector<unsigned int>&) noexcept;
    ICST(const ICST&) = delete;
    ICST(ICST&&) = default;

    Node next() noexcept;
};