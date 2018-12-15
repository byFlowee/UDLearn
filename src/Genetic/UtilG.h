#ifndef _UtilG_
#define _UtilG_

#include <cstdlib>
#include <vector>

#include "../NeuralNetwork/mat.h"
#include "../NeuralNetwork/neuralNetwork.h"

class UtilG {
    public:
        static Mat getRandomMatrix(size_t, size_t);
        static double getRandomDouble(double min, double max) { return min + ((double)rand() / RAND_MAX) * (max - min); };
        static Mat flattenMatices(const vector<Mat>&);
        static Mat unionOfFlattenedMatrices(const Mat&, const Mat&);
        static vector<vector<Mat>> unflattenMatrices(const vector<int>&, const Mat&);
        static Mat getARepresentativeVectorOfNeuralNetwork(const NeuralNetwork&);
        static Mat getRandomRepresentativeVectorOfNeuralNetwork(const vector<int>&);
        static vector<vector<Mat>> setRepresentativeVectorOnNeuralNetwork(const Mat&, NeuralNetwork&);
        static bool compareDouble(double, double);
        static bool compareDouble(const vector<double>&, const vector<double>&);
        static bool compareDouble(const Mat&, const Mat&);
};

#endif