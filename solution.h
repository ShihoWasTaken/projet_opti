#ifndef SOLUTION_H
#define SOLUTION_H

    #include <iostream>

    using namespace std;

    class Solution
    {
        public:
            Solution();
            Solution(double distance, double cost);
            virtual ~Solution();
            double Getdistance() { return m_distance; }
            void Setdistance(double val) { m_distance = val; }
            double Getcost() { return m_cost; }
            void Setcost(double val) { m_cost = val; }
            bool Getdominated() { return m_dominated; }
            void Setdominated(bool val) { m_dominated = val; }
        protected:
        private:
            double m_distance;
            double m_cost;
            bool m_dominated;
    };

#endif // SOLUTION_H
