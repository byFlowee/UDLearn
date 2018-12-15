COMPILER := g++
OPTIONS := -Wall
TARGET   := main
OBJ 	 := GeneticNN.o DNA.o UtilG.o mat.o neuralNetwork.o Player.o main.o

main: $(OBJ)
	$(COMPILER) $(OPTIONS) $(OBJ) -o $(TARGET)

main.o: main.cpp
	$(COMPILER) $(OPTIONS) -c main.cpp

mat.o: ../NeuralNetwork/mat.h ../NeuralNetwork/mat.cpp
	$(COMPILER) $(OPTIONS) -c ../NeuralNetwork/mat.cpp

neuralNetwork.o: ../NeuralNetwork/neuralNetwork.h ../NeuralNetwork/neuralNetwork.cpp
	$(COMPILER) $(OPTIONS) -c ../NeuralNetwork/neuralNetwork.cpp

UtilG.o: UtilG.h UtilG.cpp ../NeuralNetwork/mat.h ../NeuralNetwork/neuralNetwork.h
	$(COMPILER) $(OPTIONS) -c UtilG.cpp

DNA.o: DNA.h DNA.cpp UtilG.h
	$(COMPILER) $(OPTIONS) -c DNA.cpp

GeneticNN.o: GeneticNN.h GeneticNN.cpp DNA.h UtilG.h ../NeuralNetwork/neuralNetwork.h Player.h
	$(COMPILER) $(OPTIONS) -c GeneticNN.cpp

Player.o: Player.h Player.cpp ../NeuralNetwork/neuralNetwork.h
	$(COMPILER) $(OPTIONS) -c Player.cpp

clean:
	rm -rvf *.o $(TARGET) 