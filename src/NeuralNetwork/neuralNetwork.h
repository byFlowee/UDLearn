
#ifndef __NEURALNETWORK__
#define __NEURALNETWORK__

using namespace std;

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "mat.h"

class NeuralNetwork
{

    private:
        vector<int> size;
        vector<double> dropout;
        vector<Mat> weights;
        vector<Mat> bias;
        vector<Mat> a;
        vector<Mat> da;
        vector<Mat> dropoutMats;
        double learningRate;
        Mat error;

        double sigmoid(double d) const;
        double sigmoidPrime(double d) const;
        double activationFunction(double d) const;
        double activationFunctionPrime(double d) const;

    public:
        NeuralNetwork(const vector<int> &size);
        NeuralNetwork(const vector<int> &size, const vector<double> &dropout);
        void initialize();
        void setLearningRate(double learningRate);
        Mat forwardPropagation(const Mat &initial, bool training = false);
        void backPropagation(const Mat &inputs, const Mat &expectedOutputs);
        void train(const vector<Mat> &inputs, const vector<Mat> &expectedOutputs, int iterations);
        string description() const;
        vector<Mat> getWeights() const;
        vector<Mat> getBias() const;
        void setWeights(const vector<Mat>&);
        void setBias(const vector<Mat>&);
        vector<int> getTopology() const;
        void updateDropoutMats();
        void setDropout(vector<double> &dropout) { this->dropout = dropout; }
        void crossFoldValidation(unsigned epochs, unsigned folds, string filename);
        static vector<string> NeuralNetwork::getNextLineAndSplitIntoTokens(istream& str);
        double getTotalError(const vector<Mat> &inputs, const vector<Mat> &expectedOutputs);

};

#endif