#pragma once

#include <vector>

class ICST {
private:
    std::vector<std::vector<unsigned int>> nodes_;
public:
    ICST(const std::vector<unsigned int>&) noexcept;
    ICST(const ICST&) = delete;
    ICST(ICST&&) = default;

    std::vector<unsigned int> next() noexcept;
};