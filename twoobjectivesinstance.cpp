#include "twoobjectivesinstance.h"

TwoObjectivesInstance::TwoObjectivesInstance()
{
    //ctor
}

TwoObjectivesInstance::TwoObjectivesInstance(string filename1, string filename2)
:m_File1Path(filename1), m_File2Path(filename2)
{
    // On parse le premier fichier
    this->m_File1Matrix = this->parsingTSPFile(filename1, &this->m_File1Dimension);
    // On parse le deuxième fichier
    this->m_File2Matrix = this->parsingTSPFile(filename2, &this->m_File2Dimension);

    // On teste si les dimensions des deux fichiers sont les mêmes
    if(this->m_File1Dimension != this->m_File2Dimension)
    {
        cout << "Erreur: les deux fichiers n'ont pas le même nombre de villes" << endl;
        cout << filename1 << " possède " << this->m_File1Dimension << " villes"<< endl;
        cout << filename2 << " possède " << this->m_File2Dimension << " villes"<< endl;
        exit(EXIT_FAILURE);
    }
    unsigned int *itineraire;
    itineraire = this->randomRoute(m_File1Dimension, 0, true);
    cout << "L'itinéraire est: " << endl;
    int a, b;
    for(int i = 0; i < m_File1Dimension; i++)
    {
        a = itineraire[i-1] - 1;
        b = itineraire[i] - 1;
        if( (i!=0) && (i !=(m_File1Dimension-1)) )
            cout << " Ville N°" << itineraire[i-1] << " => " << "Ville N°" << itineraire[i] << "(" << this->m_File1Matrix[a][b] << "," << this->m_File2Matrix[a][b] << ")" << endl;
        if(i == (m_File1Dimension-1))
            cout << " Ville N°" << itineraire[i-1] << " => " << "Ville N°" << itineraire[0] << "(" << this->m_File1Matrix[a][0] << "," << this->m_File2Matrix[a][0] << ")" << endl;
    }
}

TwoObjectivesInstance::~TwoObjectivesInstance()
{
    /*
   for (int i = 0; i < m_File1Dimension; i++)
      delete [] m_File1Matrix[i];
   delete [] m_File1Matrix;
   */
}

unsigned int ** TwoObjectivesInstance::parsingTSPFile(string filename, unsigned int *dimension)
{
    unsigned int **matrix;
    ifstream fichier("files/" + filename, ios::in);
	if(!fichier)
	{
        cerr << "Impossible d'ouvrir le fichier." << endl;
	}
	else
	{
        string ligne;
        vector<string> v;

		// Premières lignes, on attends de récupérer le nombre de villes
		do
		{
            getline(fichier, ligne);
            v = this->explode(ligne, ' ');
		}while(v[0] != "DIMENSION:");
		*dimension = stoi(v[1]);

		// On passe les lignes jusqu'à arriver à la section des coordonées
        do
		{
            getline(fichier, ligne);
		}while(ligne != "NODE_COORD_SECTION");

        // Ici on peut commencer à lire les coordonées

        // On alloue la matrice des x
        unsigned int x[*dimension];

        // On alloue la matrice des y
        unsigned int y[*dimension];

        for(int i = 0; i < *dimension; i++)
        {
            getline(fichier, ligne);
            cout << ligne << endl;

            v = this->explode(ligne, ' ');
            x[i] = stoi(v[1]);
            y[i] = stoi(v[2]);
        }

        // On alloue la matrice
        matrix = new unsigned int* [*dimension];
        for (int i = 0; i < *dimension; i++)
            matrix[i] = new unsigned int[*dimension];

        // On remplit la matrice qu'on a précédemment allouée
        for(int i = 0; i < *dimension; i++)
        {
            for(int j = 0; j < *dimension; j++)
            {
                // Cas Ville A / Ville A (symétrique)
                if(i == j)
                {
                    matrix[i][j] = 0;
                }
                // Autre cas (normal)
                else
                {
                    matrix[i][j] = distanceBetweenCities(x[i], y[i], x[j], y[j]);
                }
                cout << "[" << i+1 << "]" << "[" << j+1 << "] = " << matrix[i][j] << " Km"<< endl;
            }
        }

        // On ferme le  fichier
		fichier.close();
	}
	return matrix;
}

float TwoObjectivesInstance::distanceBetweenCities(int x1, int y1, int x2, int y2)
{
    return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

string TwoObjectivesInstance::generateSolution()
{

}

// Retourne un tableau des villes classées aléatoirement (de 1 à n) pas de (0 à n-1)
unsigned int* TwoObjectivesInstance::randomRoute(unsigned int dimension, int iteration, bool init)
{
    if (init)
    {
        m_seeds[iteration] = std::time(0);
    }
    std::srand(m_seeds[iteration]);

    bool picked[dimension];
    unsigned int *villes = new unsigned int[dimension];
    for(int i = 0; i < dimension; i++)
    {
        picked[i] = false;
    }
    int random_variable;
    bool ok = false;
    for(int i = 0; i < dimension; i++)
    {
        ok = false;
        do
        {
            random_variable = rand() % dimension + 1;
            if(picked[random_variable-1] != true)
            {
                picked[random_variable-1] = true;
                ok = true;
            }
        }while(!ok);
        villes[i] = random_variable;
    }
    return villes;
}


vector<string> TwoObjectivesInstance::explode(string const & s, char delim)
{
    vector<string> result;
    istringstream iss(s);

    for (string token; getline(iss, token, delim); )
    {
        result.push_back(move(token));
    }

    return result;
}
