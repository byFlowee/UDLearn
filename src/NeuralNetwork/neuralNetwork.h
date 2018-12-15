
#ifndef __NEURALNETWORK__
#define __NEURALNETWORK__

using namespace std;

#include <vector>
#include <string>

#include "mat.h"

class NeuralNetwork
{

    private:
        vector<int> size;
        vector<Mat> weights;
        vector<Mat> bias;
        vector<Mat> a;
        vector<Mat> da;
        double learningRate;

        double sigmoid(double d) const;
        double sigmoidPrime(double d) const;
        double activationFunction(double d) const;
        double activationFunctionPrime(double d) const;

    public:
        NeuralNetwork(const vector<int> &size);
        void initialize();
        void setLearningRate(double learningRate);
        Mat forwardPropagation(const Mat &initial);
        void backPropagation(const Mat &inputs, const Mat &expectedOutputs);
        void train(const vector<Mat> &inputs, const vector<Mat> &expectedOutputs, int iterations);
        string description() const;
        vector<Mat> getWeights() const;
        vector<Mat> getBias() const;
        void setWeights(const vector<Mat>&);
        void setBias(const vector<Mat>&);
        vector<int> getTopology() const;

};

#endif