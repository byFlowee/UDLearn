#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <vector>
#include <iostream>
#include "Perceptron.h"
using namespace std;

class Network {
    public:
        Network(const vector<unsigned> &topology);
        void feedForward(const vector<double> &input);
        void backPropagation(const vector<double> &target);
        void getResults(vector<double> &result) const;
        void printTopology() const;
        double getRecentAverageError() const { return avgError; }
    private:
        vector<vector<Perceptron>> layers;
        double loss;
        double avgError;
        static double smoothingFactor;
};

#endif