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

vector<unsigned int> Solution::twoOpt(vector<unsigned int> v, unsigned int index1, unsigned int index2)
{
    vector<unsigned int> voisine;
    int j = 0;
    for(int i = 0; i < v.size() ; i++)
    {
        if( !(i < index1) && !(i > index2) )
        {
            voisine.push_back(v.at(index2 - j++));
        }
        else
        {
            voisine.push_back(v.at(i));
        }
    }
    return voisine;
}

//TODO
vector<Solution> Solution::GenerateVoisinage()
{
    vector<Solution> tmp;
    return tmp;
}


Solution::~Solution()
{
    //dtor
}
