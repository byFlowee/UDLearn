#ifndef _UtilG_
#define _UtilG_

#include <cstdlib>

#include "../NeuralNetwork/mat.h"

class UtilG {
    public:
        static Mat getRandomMatrix(unsigned, unsigned);
        static double getRandomDouble(double min, double max) { return min + ((double)rand() / RAND_MAX) * (max - min); };

};

#endif