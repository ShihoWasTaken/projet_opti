#include "twoobjectivesinstance.h"

TwoObjectivesInstance::TwoObjectivesInstance()
{
    //ctor
}

TwoObjectivesInstance::TwoObjectivesInstance(string name, string filename1, string filename2)
:m_Name(name), m_File1Path(filename1), m_File2Path(filename2)
{
    // On initialise srand
    std::srand(std::time(0));

    // On établit la précision
    //cout.precision(dbl::max_digits10);
    cout.precision(8);

    // On parse les fichiers
    this->m_File1Matrix = this->parsingTSPFile(filename1, &this->m_File1Dimension);
    this->m_File2Matrix = this->parsingTSPFile(filename2, &this->m_File2Dimension);

    // On teste si les dimensions des deux fichiers sont les mêmes
    // Si ce n'est pas le cas, il y'a un problème
    if(this->m_File1Dimension != this->m_File2Dimension)
    {
        cout << "Erreur: les deux fichiers n'ont pas le même nombre de villes" << endl;
        cout << filename1 << " possède " << this->m_File1Dimension << " villes"<< endl;
        cout << filename2 << " possède " << this->m_File2Dimension << " villes"<< endl;
        exit(EXIT_FAILURE);
    }

    // On génère 500 solutions
    for(int i = 0; i < SOLUTIONS; i++)
    {
        cout << endl << "Solution N°" << i+1 << endl;
        this->generateSolution(i);
//        this->checkDominance(i);
    }
    offline();
    // On affiche les 500 solutions
    for(int i = 0; i < SOLUTIONS; i++)
    {
        cout << endl << "Solution N°" << i+1 << endl;
        if(this->m_solutions[i].Getdominated())
        {
            cout << "Distance: " << this->m_solutions[i].Getdistance() << " - Coût: " << this->m_solutions[i].Getcost() << endl;
            cout << "Cette solution est dominée" << endl;
        }
        else
            cout << "Distance: " << this->m_solutions[i].Getdistance() << " - Coût: " << this->m_solutions[i].Getcost() << endl;

    }
}

TwoObjectivesInstance::~TwoObjectivesInstance()
{

}

// Une solution est dominée si sa distance et son cout sont tous les deux plus grand que ceux qu'une autre solution
void TwoObjectivesInstance::checkDominance(int iterationMax)
{
    for(int i = 0; i < iterationMax; i++)
    {
        if( (m_solutions[iterationMax].Getdistance() < m_solutions[i].Getdistance()) && (m_solutions[iterationMax].Getcost() < m_solutions[i].Getcost()) )
            this->m_solutions[i].Setdominated(true);
    }
}

void TwoObjectivesInstance::offline()
{
    for(int i = 0; i < SOLUTIONS; ++i)
    {
        for(int j = i+1; j < SOLUTIONS; ++j)
        {
            if((m_solutions[i].Getdistance() > m_solutions[j].Getdistance())&&(m_solutions[i].Getcost() > m_solutions[j].Getcost()))
            {
                m_solutions[i].Setdominated(true);
            }
            else if ((m_solutions[i].Getdistance() < m_solutions[j].Getdistance())&&(m_solutions[i].Getcost() < m_solutions[j].Getcost()))
            {
                m_solutions[j].Setdominated(true);
            }
        }
    }
}


// Fonction de parsing du fichier
double ** TwoObjectivesInstance::parsingTSPFile(string filename, unsigned int *dimension)
{
    double **matrix;
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
            v = this->explode(ligne, ' ');
            x[i] = stoi(v[1]);
            y[i] = stoi(v[2]);
        }

        // On alloue la matrice
        matrix = new double* [*dimension];
        for (int i = 0; i < *dimension; i++)
            matrix[i] = new double[*dimension];

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
               //cout << "[" << i+1 << "]" << "[" << j+1 << "] = " << matrix[i][j] << " Km"<< endl;
            }
        }

        // On ferme le  fichier
		fichier.close();
	}
	return matrix;
}

// Retourne la distance ou le coût entre 2 villes
double TwoObjectivesInstance::distanceBetweenCities(int x1, int y1, int x2, int y2)
{
    return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

// Génère une solution
void TwoObjectivesInstance::generateSolution(int iteration)
{
    unsigned int *itineraire;
    itineraire = this->randomRoute(m_File1Dimension, iteration);
    cout << "L'itinéraire est: " << endl;
    int a, b;
    double total1 = 0;
    double total2 = 0;
    for(int i = 0; i < m_File1Dimension; i++)
    {
        a = itineraire[i-1];
        b = itineraire[i];

        // Toutes les itérations sauf la première et la dernière
        if( (i!=0) && (i !=(m_File1Dimension-1)) )
        {
            //cout << " Ville N°" << a << " => " << "Ville N°" << b << " (" << this->m_File1Matrix[a-1][b-1] << " km," << this->m_File2Matrix[a-1][b-1] << " €)" << endl;
            total1 += (double) this->m_File1Matrix[a-1][b-1];
            total2 += (double) this->m_File2Matrix[a-1][b-1];
        }

        // Dernière itération, on ferme la boucle. On va de la dernière ville jusqu'à la ville de départ
        if(i == (m_File1Dimension-1))
        {
            //cout << " Ville N°" << a << " => " << "Ville N°" << itineraire[0] << " (" << this->m_File1Matrix[a-1][0] << " km," << this->m_File2Matrix[a-1][0] << " €)" << endl;
            total1 += (double) this->m_File1Matrix[a-1][0];
            total2 += (double) this->m_File2Matrix[a-1][0];
        }
    }
    this->m_solutions[iteration] = Solution(total1, total2);
    cout << "Distance totale: " << total1 << " Km" << endl;
    cout << "Coût total: " << total2 << " €" << endl;
}


// Retourne un tableau des villes classées aléatoirement (de 1 à n) pas de (0 à n-1)
unsigned int* TwoObjectivesInstance::randomRoute(unsigned int dimension, int iteration)
{



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

// Fonction explode pour aider le parsing du fichier
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
