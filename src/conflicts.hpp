#include <set>

enum class ConflictFollow{
    no_train,//implies no cycle or swap
    no_cycle,//implies no swap and yes to train
    no_swap,//implies yes to train
    yes_all
};

struct conflicts
{
    bool collision;
    ConflictFollow follow;
};