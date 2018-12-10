#ifndef _PERCEPTRON_
#define _PERCEPTRON_

#include <vector>
#include <cstdlib>
#include <string>

using namespace std;;

class Perceptron{
    private:
        vector<double> inputs;
        vector<double> weights;
        double output;
        static double getRandomWeight() { return -1.0 + ((double)rand() / RAND_MAX) * 2.0; }

    public:
        Perceptron(unsigned);
        void computeOutput();
        double getOutput();
        void updateWeights(double target);
        double getPrediction() const;
        void trainPerceptron(unsigned epochs, const vector<vector<double> > &inputs, const vector<double> &targets);
};

#endif