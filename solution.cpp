#include "solution.h"

Solution::Solution()
{
    //ctor
}

Solution::Solution(double distance, double cost)
:m_distance(distance), m_cost(cost), m_dominated(false)
{
    //ctor
}

Solution::~Solution()
{
    //dtor
}
