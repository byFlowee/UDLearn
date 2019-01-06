COMPILER := g++
OPTIONS  := -Wall
TARGET   := main
OBJ 	 := main.o GANN.o GANNWrapper.o DNA.o UtilG.o mat.o neuralNetwork.o Player.o
OBJCPP 	 := main.cpp GANN.cpp GANNWrapper.cpp DNA.cpp UtilG.cpp ../NeuralNetwork/mat.cpp ../NeuralNetwork/neuralNetwork.cpp Player.cpp



USE_SDL := 1

# This will likely need to be changed to suit your installation.
#ALE := ../../../ALE
ALE := $(ALEPath)

FLAGS   := -I$(ALE)/src -I$(ALE)/src/controllers -I$(ALE)/src/os_dependent -I$(ALE)/src/environment -I$(ALE)/src/external -L$(ALE)
CXX     := $(COMPILER)
FILE    := $(TARGET)
NETWORK := ../NeuralNetwork/mat.cpp ../NeuralNetwork/neuralNetwork.cpp
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

GANN.o: GANN.h GANN.cpp DNA.h UtilG.h ../NeuralNetwork/neuralNetwork.h Player.h
	$(COMPILER) $(OPTIONS) -c GANN.cpp

GANNWrapper.o: GANNWrapper.h GANNWrapper.cpp GANN.h ../NeuralNetwork/neuralNetwork.h
	$(COMPILER) $(OPTIONS) -c GANNWrapper.cpp

Player.o: Player.h Player.cpp
	$(CXX) $(DEFINES) $(FLAGS) $(NETWORK) $(LDFLAGS) -c Player.cpp

clean:
	rm -rvf *.o $(TARGET) 