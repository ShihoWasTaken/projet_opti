#ifndef SOLUTION_H
#define SOLUTION_H

    #include <iostream>

    using namespace std;

    class Solution
    {
        public:
            Solution();
            Solution(float distance, float cost);
            virtual ~Solution();
            float Getdistance() { return m_distance; }
            void Setdistance(float val) { m_distance = val; }
            float Getcost() { return m_cost; }
            void Setcost(float val) { m_cost = val; }
            bool Getdominated() { return m_dominated; }
            void Setdominated(bool val) { m_dominated = val; }
        protected:
        private:
            float m_distance;
            float m_cost;
            bool m_dominated;
    };

#endif // SOLUTION_H
