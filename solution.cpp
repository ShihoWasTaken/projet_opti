#include "solution.h"

Solution::Solution()
{
    //ctor
}

Solution::Solution(float distance, float cost)
:m_distance(distance), m_cost(cost), m_dominated(false)
{
    //ctor
}

Solution::~Solution()
{
    //dtor
}
