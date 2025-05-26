#include <set>

enum class ConflictFollow{
    no_train,
    no_cycle,
    no_swap,
    yes_all
};

struct conflicts
{
    bool collision;
    ConflictFollow follow;
};