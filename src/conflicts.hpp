#include <set>

enum class conflict_types{
    collision,
    swap,
    train,
    cycle
};

using conflicts=std::set<conflict_types>;