main: main.o mat.o neuralNetwork.o
	g++ -Wall -O3 main.o mat.o neuralNetwork.o -o main

main.o: main.cpp
	g++ -Wall -O3 -c main.cpp

mat.o: mat.h mat.cpp
	g++ -Wall -O3 -c mat.cpp

neuralNetwork.o: neuralNetwork.h neuralNetwork.cpp
	g++ -Wall -O3 -c neuralNetwork.cpp