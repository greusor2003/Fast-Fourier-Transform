main:
	g++ -std=c++17 -O3 -Wall -pedantic -pthread -fopenmp main.cpp -o main
clean:
	rm -f main
