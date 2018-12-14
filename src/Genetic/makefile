COMPILER := g++
CXXFLAGS := -Wall
TARGET   := test
OBJ 	 := UtilG.o DNA.o GeneticNN.o

main: GeneticNN.o DNA.o UtilG.o main.cpp
	$(COMPILER) $(CXXFLAGS) $(OBJ) main.cpp -o $(TARGET)

UtilG.o: UtilG.h UtilG.cpp
	$(COMPILER) $(CXXFLAGS) -I../NeuralNetwork/mat.o -c UtilG.cpp

DNA.o: DNA.h DNA.cpp UtilG.h
	$(COMPILER) $(CXXFLAGS) -c DNA.cpp

GeneticNN.o: GeneticNN.h GeneticNN.h
	$(COMPILER) $(CXXFLAGS) -c GeneticNN.cpp

clean:
	rm -rvf *.o $(TARGET) 