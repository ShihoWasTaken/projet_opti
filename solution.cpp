#include "solution.h"

Solution::Solution()
{
    //ctor
}

Solution::Solution(double distance, double cost)
:m_distance(distance), m_cost(cost), m_onlineDominated(false), m_offlineDominated(false)
{
    //ctor
}

Solution::~Solution()
{
    //dtor
}
