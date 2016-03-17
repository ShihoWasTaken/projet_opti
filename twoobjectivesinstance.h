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

    using namespace std;

    class TwoObjectivesInstance
    {
        public:
            TwoObjectivesInstance();
            TwoObjectivesInstance(string filename1, string filename2);
            virtual ~TwoObjectivesInstance();
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

            unsigned int** GetFile1Matrix() { return m_File1Matrix; }
            void SetFile1Matrix(unsigned int** val) { m_File1Matrix = val; }
            unsigned int** GetFile2Matrix() { return m_File2Matrix; }
            void SetFile2Matrix(unsigned int** val) { m_File2Matrix = val; }
            string generateSolution();
            unsigned int* randomRoute(unsigned int dimension, int iteration, bool init);
        protected:
        private:
            unsigned int ** parsingTSPFile(string filename,  unsigned int *dimension);
            vector<string> explode(string const & s, char delim);
            float distanceBetweenCities(int x1, int y1, int x2, int y2);


            string m_Name;
            string m_File1Path;
            string m_File2Path;
            unsigned int m_seeds[500];
            unsigned int m_File1Dimension;
            unsigned int m_File2Dimension;
            unsigned int **m_File1Matrix;
            unsigned int **m_File2Matrix;
    };

#endif // TWOOBJECTIVESINSTANCE_H
