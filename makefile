TSP: main.o twoobjectivesinstance.o solution.o
	g++ -o TSP main.o twoobjectivesinstance.o solution.o -std=c++11
	make clean
main.o: main.cpp solution.cpp twoobjectivesinstance.cpp
	g++ -c main.cpp -std=c++11
twoobjectivesinstance.o: solution.cpp twoobjectivesinstance.cpp
	g++ -c twoobjectivesinstance.cpp -std=c++11
solution.o: solution.cpp
	g++ -c solution.cpp -std=c++11
clean:
	rm *.o