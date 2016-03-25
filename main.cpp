#include <iostream>
#include <string>
#include "twoobjectivesinstance.h"

using namespace std;

double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

int main()
{
    double cpu0  = get_cpu_time();
    // Syntaxe: TwoObjectivesInstance(nom_de_l_instance, nom_fichier_1, nom_fichier_2)

    // Pour tester les classes
    TwoObjectivesInstance meow("kroAB100", "kroA100.tsp", "kroB100.tsp");

    // Pour quand toutes les fonctions seront finalisées
    /*
    TwoObjectivesInstance instance[7] =
    {
        TwoObjectivesInstance("kroAB100", "kroA100.tsp", "kroB100.tsp"),
        TwoObjectivesInstance("kroBC100", "kroB100.tsp", "kroC100.tsp"),
        TwoObjectivesInstance("kroCD100", "kroC100.tsp", "kroD100.tsp"),
        TwoObjectivesInstance("kroDE100", "kroD100.tsp", "kroE100.tsp"),
        TwoObjectivesInstance("kroAE100", "kroA100.tsp", "kroE100.tsp"),
        TwoObjectivesInstance("kroAB150", "kroA150.tsp", "kroB150.tsp"),
        TwoObjectivesInstance("kroAB200", "kroA200.tsp", "kroB200.tsp")
    };
    */
    double cpu1  = get_cpu_time();
    cout << "Temps d'exécution : " << (int)( (cpu1  - cpu0) * 1000) << " milliseconds (CPU time)"<< endl;

    return EXIT_SUCCESS;
}
