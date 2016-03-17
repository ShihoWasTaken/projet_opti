#include <iostream>
#include <string>
#include "twoobjectivesinstance.h"

using namespace std;

int main()
{
    string files[] =
    {
        "kroA100.tsp",
        "kroB100.tsp",
        "kroC100.tsp",
        "kroD100.tsp",
        "kroE100.tsp",
        "kroA150.tsp",
        "kroB150.tsp",
        "kroA200.tsp",
        "kroB200.tsp"
    };

    TwoObjectivesInstance instances[9];
    TwoObjectivesInstance instance("kroA100.tsp", "kroB100.tsp");


    cout << "Hello world!" << endl;
    return 0;
}
