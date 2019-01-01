#ifndef _UtilG_
#define _UtilG_

#include <cstdlib>
#include <vector>
#include <fstream>

#include "../NeuralNetwork/mat.h"
#include "../NeuralNetwork/neuralNetwork.h"
#include "GeneticNN.h"

class UtilG {
    public:
        static Mat getRandomMatrix(size_t, size_t, size_t = 1);
        static Mat getRandomMatrix(size_t, size_t, const vector<WeightInitializationRange>&);
        static double getRandomDouble(double min, double max);
        static Mat flattenMatices(const vector<Mat>&);
        static Mat unionOfFlattenedMatrices(const Mat&, const Mat&);
        static vector<vector<Mat>> unflattenMatrices(const vector<int>&, const Mat&);
        static Mat getARepresentativeVectorOfNeuralNetwork(const NeuralNetwork&);
        static Mat getRandomRepresentativeVectorOfNeuralNetwork(const vector<int>&);
        static vector<vector<Mat>> setRepresentativeVectorOnNeuralNetwork(const Mat&, NeuralNetwork&);
        static Mat getMatFromVector(const vector<double>&);
        static bool compareDouble(double, double);
        static bool compareDouble(const vector<double>&, const vector<double>&);
        static bool compareDouble(const Mat&, const Mat&);
        static void printVector(const vector<int>&, ofstream&, string = ", ");
};

#endif