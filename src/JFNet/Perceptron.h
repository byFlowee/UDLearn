#ifndef _PERCEPTRON_H_
#define _PERCEPTRON_H_

#include <cstdlib>
#include <cmath>
#include <vector>
#include <time.h>

using namespace std;

class Perceptron {
    public:
        Perceptron(unsigned nOutputs, unsigned index);
        void computeOutput(const vector<Perceptron> &prev);
        double getOutput() const { return this->output; }
        void setOutput(double output) { this->output = output; }
        void computeOutputGradients(double target);
        void computeHiddenGradients(vector<Perceptron> &nextLayer);
        void updateWeights(vector<Perceptron> &previousLayer); 
    
    private:
        unsigned index; 
        double output;
        double gradient;
        static double lr;
        static double momentum;
        vector<double> weights;
        vector<double> deltaWeights;
        static double randomWeightInit() { return (double) rand() / double(RAND_MAX); }
        static double activationFunction(double x);
        static double activationFunctionDerivate(double x);
};

#endif