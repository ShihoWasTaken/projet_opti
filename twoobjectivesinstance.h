#ifndef TWOOBJECTIVESINSTANCE_H
#define TWOOBJECTIVESINSTANCE_H

    #include <iostream>
    #include <string>
    #include <vector>
    #include <sstream>
    #include <utility>
    #include <fstream>
    #include <cmath>
    #include <numeric>
    #include <random>
    #include <algorithm>
    #include <iterator>
    #include <ctime>
    #include <limits>
    #include "solution.h"

    #define SOLUTIONS 500
    #define MAX 100
    #define SHOW_DEBUGS 0

    using namespace std;

    // Un typedef pour établier la précision des float/double
    typedef std::numeric_limits< double > dbl;

    // Définition d'une enum pour le filtrage
    enum Filtrage { OFFLINE, ONLINE};

    /*  Cette classe représente une instance à étudier.
        Une instance possède:
            - m_Name | un nom (exemple: kroAB100)
            - m_File1Path | un fichier correspondant aux distances entre les villes
            - m_File2Path | un fichier correspondant aux coûts entre les villes
            - m_File1Dimension | le nombre de villes du fichier 1
            - m_File2Dimension | le nombre de villes du fichier 2
            - m_File1Matrix | la matrice des distances entre chaque villes
            - m_File2Matrix | la matrice des coût entre chaque villes
            - m_solutions | un tableau contenant les 500 solutions de l'instance
            - m_DominatedSolutionsCount | le nombre de solutions dominées
    */
    class TwoObjectivesInstance
    {
        public:
            // Constructeurs / Destructeurs
            TwoObjectivesInstance();
            TwoObjectivesInstance(string name, string filename1, string filename2);
            virtual ~TwoObjectivesInstance();

            // Getters / Setters
            string GetName() { return m_Name; }
            void SetName(string val) { m_Name = val; }
            string GetFile1Path() { return m_File1Path; }
            void SetFile1Path(string val) { m_File1Path = val; }
            string GetFile2Path() { return m_File2Path; }
            void SetFile2Path(string val) { m_File2Path = val; }
            unsigned int GetFile1Dimension() { return m_File1Dimension; }
            void SetFile1Dimension(unsigned int val) { m_File1Dimension = val; }
            unsigned int GetFile2Dimension() { return m_File2Dimension; }
            void SetFile2Dimension(unsigned int val) { m_File2Dimension = val; }
            double** GetFile1Matrix() { return m_File1Matrix; }
            void SetFile1Matrix(double** val) { m_File1Matrix = val; }
            double** GetFile2Matrix() { return m_File2Matrix; }
            void SetFile2Matrix(double** val) { m_File2Matrix = val; }

            // Autres fonctions membres publiques
            void generateSolution(int iteration);
            unsigned int* randomRoute(unsigned int dimension, int iteration);
        protected:
        private:
            // Fonctions membres privées
            double ** parsingTSPFile(string filename,  unsigned int *dimension);
            vector<string> explode(string const & s, char delim);
            double get_cpu_time();
            double distanceBetweenCities(int x1, int y1, int x2, int y2);
            void checkDominance(int iterationMax);
            void offlineFiltering();
            void onlineFiltering();
            void saveSolutions(string filename);
            void savePareto(string filename, Filtrage filtrage);
            void makePlot(string filename, Filtrage filtrage, bool isPareto);
            void makePlotForPLS();
            vector<Solution> PLS();
            bool OnlineFilteringForPLS(vector <Solution> &best_sols, Solution n);
            vector<unsigned int> twoOpt(vector<unsigned int> v, unsigned int index1, unsigned int index2);
            vector<Solution> GenerateVoisinage(Solution s);
            void sumVilles(Solution &s);
            void createApproxFile();
            void fillApproxFile(vector <Solution> best_sols, int iteration);

            // Attributs
            string m_Name;
            string m_File1Path;
            string m_File2Path;
            unsigned int m_File1Dimension;
            unsigned int m_File2Dimension;
            double **m_File1Matrix;
            double **m_File2Matrix;
            unsigned int m_DominatedSolutionsCount;
            Solution m_solutions[SOLUTIONS];
    };

#endif // TWOOBJECTIVESINSTANCE_H
