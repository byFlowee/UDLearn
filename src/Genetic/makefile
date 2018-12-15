COMPILER := g++
OPTIONS := -Wall
TARGET   := main
OBJ 	 := GeneticNN.o DNA.o UtilG.o mat.o neuralNetwork.o Player.o main.o
OBJCPP 	 := GeneticNN.cpp DNA.cpp UtilG.cpp ../breakout/DataLoader.cpp ../JNet/JNet.cpp ../JNet/Perceptron.cpp ../NeuralNetwork/mat.cpp ../NeuralNetwork/neuralNetwork.cpp Player.cpp main.cpp



USE_SDL := 1

# This will likely need to be changed to suit your installation.
ALE := ../../../ALE

FLAGS := -I$(ALE)/src -I$(ALE)/src/controllers -I$(ALE)/src/os_dependent -I$(ALE)/src/environment -I$(ALE)/src/external -L$(ALE)
CXX := $(COMPILER)
FILE := $(TARGET)
NETWORK := ../NeuralNetwork/mat.cpp ../NeuralNetwork/neuralNetwork.cpp ../JNet/JNet.cpp ../JNet/Perceptron.cpp
LDFLAGS := -lale -lz

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    FLAGS += -Wl,-rpath=$(ALE)
endif
ifeq ($(UNAME_S),Darwin)
    FLAGS += -framework Cocoa
endif

ifeq ($(strip $(USE_SDL)), 1)
  DEFINES += -D__USE_SDL -DSOUND_SUPPORT
  FLAGS += $(shell sdl-config --cflags)
  LDFLAGS += $(shell sdl-config --libs)
endif



main: $(OBJ)
	#$(CXX) $(DEFINES) $(FLAGS) $(LDFLAGS) $(OBJ) -o $(TARGET)
	$(CXX) $(DEFINES) $(FLAGS) $(OBJCPP) $(LDFLAGS) -o $(TARGET)

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

#Player.o: Player.h Player.cpp
	#$(COMPILER) $(OPTIONS) -c Player.cpp

Player.o:
	$(CXX) $(DEFINES) $(FLAGS) $(NETWORK) $(LDFLAGS) -c Player.cpp

clean:
	rm -rvf *.o $(TARGET) 