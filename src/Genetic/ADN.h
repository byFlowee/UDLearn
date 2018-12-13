#ifndef _ADN_
#define _ADN_

#include <vector>
#include <string>

#include "../NeuralNetwork/mat.h"

using namespace std;

class ADN {
    private:
        double mutation;
        Mat genes;

    public:
        ADN(const Mat&, double = 0.01);
        void setMutation(double);
        ADN crossover(const ADN&);
        void mutate();
};

#endif