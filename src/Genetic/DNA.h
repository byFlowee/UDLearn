#ifndef _DNA_
#define _DNA_

#include <vector>
#include <string>

#include "../NeuralNetwork/mat.h"

using namespace std;

class DNA {
    private:
        double mutation;
        Mat genes;

    public:
        DNA();
        DNA(const Mat&, double = 0.01);
        void setMutation(double);
        DNA crossover(const DNA&);
        void mutate();
};

#endif