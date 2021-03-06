#ifndef SOLUTION_H
#define SOLUTION_H

    #include <iostream>
    #include <vector>

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
            bool GetOnlineDominated() { return m_onlineDominated; }
            void SetOnlineDominated(bool val) { m_onlineDominated = val; }
            bool GetOfflineDominated() { return m_offlineDominated; }
            void SetOfflineDominated(bool val) { m_offlineDominated = val; }
            bool GetExplored() const { return m_explored; }
            void SetExplored(bool explored) { m_explored = explored; }
            vector<unsigned int> GetItineraire() { return m_itineraire; }
            void SetItineraire(vector<unsigned int> itineraire) { m_itineraire = itineraire; }

            // Autres fonctions membres publiques

        protected:
        private:
            // Fonctions membres privées

            // Attributs
            double m_distance;  // Distance totale de l'itinéraire
            double m_cost;      // Cout total de l'itinéraire
            bool m_onlineDominated;   // Solution dominée ou non
            bool m_offlineDominated;   // Solution dominée ou non
            bool m_explored;
            vector<unsigned int> m_itineraire;
    };

#endif // SOLUTION_H
