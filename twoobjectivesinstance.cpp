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
    }

    // Filtrage Online
    this->onlineFiltering();

    // Filtrage Offline
    this->offlineFiltering();

    // On affiche les 500 solutions
    for(int i = 0; i < SOLUTIONS; i++)
    {
        cout << endl << "Solution N°" << i+1 << endl;
        cout << "Distance: " << this->m_solutions[i].Getdistance() << " - Coût: " << this->m_solutions[i].Getcost() << endl;
        cout << "Dominée online: " << this->m_solutions[i].GetOnlineDominated() << endl;
        cout << "Dominée offline: " << this->m_solutions[i].GetOfflineDominated() << endl;
    }
    // On crée un graphique sur gnuplot avec toutes les solutions
    this->makePlot(this->m_Name, OFFLINE, false);
    this->makePlot(this->m_Name, ONLINE, false);

    // On crée un graphique sur gnuplot avec le Front de Pareto
    this->makePlot(this->m_Name, OFFLINE, true);
    this->makePlot(this->m_Name, ONLINE, true);

    // 5) Fichier 10 approx front pareto
    this->createApproxFile();

    // On exécute au moins 10 fois
    for(int i = 0; i < 10; i++)
    {
        // On remplit un front de Pareto dans le fichier
        this->fillApproxFile();
    }
//    vector<unsigned int> v = {0,1,2,3,4,5,6,7,8,9,10};
//    cout << endl;
//    for(int i = 0; i < v.size(); i++)
//        cout << v.at(i) << " ";
//        cout << "Normal" << endl;
//
//        vector<unsigned int> voisine = twoOpt(v, 0, 8);
//        for(int i = 0; i < voisine.size(); i++)
//        cout << voisine.at(i) << " ";
//        cout << "Voisinagé" << endl;
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
            this->m_solutions[i].SetOnlineDominated(true);
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

// Filtre avec une stratégie Online
void TwoObjectivesInstance::onlineFiltering()
{
    for(int i = 0; i < SOLUTIONS; i++)
    {
        cout << endl << "Solution N°" << i+1 << endl;
        for(int j = 0; j <= i; j++)
        {
            if( (m_solutions[i].Getdistance() < m_solutions[j].Getdistance()) && (m_solutions[i].Getcost() < m_solutions[j].Getcost()) )
                this->m_solutions[j].SetOnlineDominated(true);
            else if ((m_solutions[i].Getdistance() > m_solutions[j].Getdistance())&&(m_solutions[i].Getcost() > m_solutions[j].Getcost()))
                this->m_solutions[i].SetOnlineDominated(true);
        }
    }
    saveSolutions("online500_" + this->m_Name + ".txt");
    savePareto("onlinePareto500_" + this->m_Name + ".txt", ONLINE);
}

// Filtre avec une stratégie Offline
void TwoObjectivesInstance::offlineFiltering()
{
    for(int i = 0; i < SOLUTIONS; ++i)
    {
        for(int j = i+1; j < SOLUTIONS; ++j)
        {
            if((m_solutions[i].Getdistance() > m_solutions[j].Getdistance())&&(m_solutions[i].Getcost() > m_solutions[j].Getcost()))
            {
                m_solutions[i].SetOfflineDominated(true);
            }
            else if ((m_solutions[i].Getdistance() < m_solutions[j].Getdistance())&&(m_solutions[i].Getcost() < m_solutions[j].Getcost()))
            {
                m_solutions[j].SetOfflineDominated(true);
            }
        }
    }
    saveSolutions("offline500_" + this->m_Name + ".txt");
    savePareto("offlinePareto500_" + this->m_Name + ".txt", OFFLINE);
}

// Génère une solution
void TwoObjectivesInstance::generateSolution(int iteration)
{
    unsigned int *itineraire;
    vector<unsigned int> villes;
    itineraire = this->randomRoute(m_File1Dimension, iteration);
    cout << "L'itinéraire est: " << endl;
    unsigned int a, b;
    double total1 = 0;
    double total2 = 0;
    for(unsigned int i = 0; i < m_File1Dimension; i++)
    {
        villes.push_back(itineraire[i]);
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
    m_solutions[iteration].SetItineraire(villes);
    cout << "Distance totale: " << total1 << " Km" << endl;
    cout << "Coût total: " << total2 << " €" << endl;
}

void TwoObjectivesInstance::PLS()
{
    vector<Solution> archive; //contient toutes les solutions non-dominées à explorer
    vector<Solution> best_sols; //set of solutions that are a Pareto local optimum

    //A rajouter dans solution : itinéraire (liste des villes), voisinage et bool explored

    //init archive : toutes les solutions de m_solutions non-dominées
    for(int i = 0; i < SOLUTIONS; ++i)
    {
        if(!m_solutions[i].GetOnlineDominated())
        {
            archive.push_back(m_solutions[i]);
        }
    }

    //best_sols = archive
    best_sols = archive;

    while(!archive.empty())
    {
        Solution s(archive.front()); //vérifier que le constructeur par recopie fonctionne...
        for(auto p : GenerateVoisinage(s))
        {
            if ((p.Getdistance() < s.Getdistance())&&(p.Getcost() < s.Getcost()))
            {
                p.SetExplored(false);
                update(best_sols, p);
            }
        }
        s.SetExplored(true);
        GetUnexplored(best_sols, archive);
    }

    //tant que !archive.empty
        //solution s = archive.front
        //voisinage = s.getVoisinage
        //pour chaque solution i de voisinage
            //si i meilleure que s alors
                //i.explored = false
                //update(best_sols, i) : on ajoute i à best_sols et on filtre best_sols
            //fin si
        //fin pour
        //s.explored = true
        //archive = getUnexplored(bestsols)
    //fin tantque
}

vector<unsigned int> TwoObjectivesInstance::twoOpt(vector<unsigned int> v, unsigned int index1, unsigned int index2)
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

vector<Solution> TwoObjectivesInstance::GenerateVoisinage(Solution s)
{
    vector<Solution> voisinage;
    for(unsigned int i = 0; i < s.GetItineraire().size()-1; ++i)
    {
        for(unsigned int j = i+1; j < s.GetItineraire().size(); ++i)
        {
            Solution p;
            p.SetItineraire(twoOpt(s.GetItineraire(), i, j));
//            TODO : peupler m_itineraire
            sumVilles(p);
            voisinage.push_back(p);
        }
    }
    return voisinage;
}


void TwoObjectivesInstance::update(vector<Solution> &best_sols, Solution s)
{
    //Filtrage online
  for (vector<Solution>::iterator i = best_sols.begin() ; i != best_sols.end(); ++i)
  {
    if(((*i).Getdistance() > s.Getdistance())&&((*i).Getcost() > s.Getcost()))
    {
        (*i).SetOnlineDominated(true);
    }
  }

  //On enlève les solutions nouvellement dominées
  for (vector<Solution>::iterator i = best_sols.begin() ; i != best_sols.end(); ++i)
  {
    if((*i).GetOnlineDominated())
    {
        best_sols.erase(i);
    }
  }

  best_sols.push_back(s);

}

void TwoObjectivesInstance::GetUnexplored(vector<Solution> best_sols, vector<Solution> &archive)
{
    archive.clear();
    for(auto b : best_sols)
    {
        if(!b.GetExplored())
        {
            archive.push_back(b);
        }
    }
}

void TwoObjectivesInstance::sumVilles(Solution &s)
{
    int a, b;
    double total1 = 0;
    double total2 = 0;
    for(int i = 0; i < m_File1Dimension; i++)
    {
        a = s.GetItineraire()[i-1];
        b = s.GetItineraire()[i];

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
    s.Setdistance(total1);
    s.Setcost(total2);
}

void TwoObjectivesInstance::createApproxFile()
{
        string filename = "10_Approximations_Pareto_" + this->m_Name + ".txt";
        ofstream fichier(filename, ios::out | ios::trunc);

        if(!fichier)
        {
            cerr << "Impossible de créer le fichier " << filename << endl;
        }
        else
        {
            fichier.close();
        }
}

void TwoObjectivesInstance::fillApproxFile()
{
        string filename = "10_Approximations_Pareto_" + this->m_Name + ".txt";
        ofstream fichier(filename, ios::out | ios::app);

        if(!fichier)
        {
            cerr << "Impossible d'ouvrir le fichier " << filename << endl;
        }
        else
        {
            for(int i = 0; i < 5; i++)
            {
                fichier << "test ligne n°" << i << endl;
            }
            fichier << endl;
            fichier.close();
        }
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

void TwoObjectivesInstance::saveSolutions(string filename)
{
        ofstream fichier(filename, ios::out | ios::trunc);

        if(!fichier)
        {
            cerr << "Impossible d'ouvrir le fichier !" << endl;
        }
        else
        {
            for(int i = 0; i < SOLUTIONS; i++)
            {
                fichier << this->m_solutions[i].Getdistance() << " " << this->m_solutions[i].Getcost() << endl;
            }
            fichier.close();
        }
}

void TwoObjectivesInstance::savePareto(string filename, Filtrage filtrage)
{
        ofstream fichier(filename, ios::out | ios::trunc);

        if(!fichier)
        {
            cerr << "Impossible d'ouvrir le fichier !" << endl;
        }
        else
        {
            for(int i = 0; i < SOLUTIONS; i++)
            {
                if(filtrage == OFFLINE)
                {
                    if(!this->m_solutions[i].GetOfflineDominated())
                    {
                        fichier << this->m_solutions[i].Getdistance() << " " << this->m_solutions[i].Getcost() << endl;
                    }
                }
                else if(filtrage == ONLINE)
                {
                    if(!this->m_solutions[i].GetOnlineDominated())
                    {
                        fichier << this->m_solutions[i].Getdistance() << " " << this->m_solutions[i].Getcost() << endl;
                    }
                }
            }
            fichier.close();
        }
}

void TwoObjectivesInstance::makePlot(string filename, Filtrage filtrage, bool isPareto)
{
    string prefix;
    if(filtrage == ONLINE)
        prefix = "online";
    else
        prefix = "offline";
    string pareto = "";
    if(isPareto)
        pareto = "Pareto";
    string input = prefix + pareto + "500_" + filename + ".txt";
    string output = prefix + pareto + "500_" + filename + ".jpg";
    string gnuplotCommand = "gnuplot -e \"";
    gnuplotCommand += "set terminal jpeg;";
    gnuplotCommand += "set output \\\"" + output + "\\\";";
    gnuplotCommand += "plot \\\"" + input + "\\\";";
    gnuplotCommand += "\"";
    system(gnuplotCommand.c_str());
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
