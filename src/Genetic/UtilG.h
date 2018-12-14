#ifndef _UtilG_
#define _UtilG_

#include <cstdlib>
#include <vector>

#include "../NeuralNetwork/mat.h"

class UtilG {
    public:
        static Mat getRandomMatrix(size_t, size_t);
        static double getRandomDouble(double min, double max) { return min + ((double)rand() / RAND_MAX) * (max - min); };
        static Mat flattenMatices(const vector<Mat>&);
        static Mat unionMatrix(const Mat&, const Mat&);
        static vector<vector<Mat>> unflattenMatrices(const vector<int>&, const Mat&);

};

#endif