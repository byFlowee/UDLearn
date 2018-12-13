COMPILER := g++
CXXFLAGS := -Wall
TARGET   := test
OBJ 	 := UtilG.o ADN.o GeneticNN.o

main: GeneticNN.o ADN.o UtilG.o main.cpp
	$(COMPILER) $(CXXFLAGS) $(OBJ) main.cpp -o $(TARGET)

UtilG.o: UtilG.h UtilG.cpp
	$(COMPILER) $(CXXFLAGS) -I../NeuralNetwork/mat.o -c UtilG.cpp

ADN.o: ADN.h ADN.cpp UtilG.h
	$(COMPILER) $(CXXFLAGS) -c ADN.cpp

GeneticNN.o: GeneticNN.h GeneticNN.h
	$(COMPILER) $(CXXFLAGS) -c GeneticNN.cpp

clean:
	rm -rvf *.o $(TARGET) 