#ifndef SOLUTION_H
#define SOLUTION_H

    #include <iostream>

    using namespace std;

    /*  Cette classe permet de stocker une solution (distance et coût de l'itinéraire).
        Elle permet également d'indiquer si une solution est dominée ou non.
    */
    class Solution
    {
        public:
            // Constructeurs / Destructeurs
            Solution();
            Solution(double distance, double cost);
            virtual ~Solution();

            // Getters / Setters
            double Getdistance() { return m_distance; }
            void Setdistance(double val) { m_distance = val; }
            double Getcost() { return m_cost; }
            void Setcost(double val) { m_cost = val; }
            bool Getdominated() { return m_dominated; }
            void Setdominated(bool val) { m_dominated = val; }

            // Autres fonctions membres publiques

        protected:
        private:
            // Fonctions membres privées

            // Attributs
            double m_distance;  // Distance totale de l'itinéraire
            double m_cost;      // Cout total de l'itinéraire
            bool m_dominated;   // Solution dominée ou non
    };

#endif // SOLUTION_H
