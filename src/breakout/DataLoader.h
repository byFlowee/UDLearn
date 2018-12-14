#ifndef _DATALOADER_H_
#define _DATALOADER_H_

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "../NeuralNetwork/mat.h"
#include "../NeuralNetwork/neuralNetwork.h"

#include "../JNet/JNet.h"
#include "../JNet/Perceptron.h"

using namespace std;

class DataLoader {
    public:
        DataLoader(string filename, vector<int> &topology);
        ~DataLoader() { delete this->net; delete this->jnet; this->net = NULL; this->jnet = NULL; }
        void trainNN(unsigned nInputs, unsigned nOutputs, unsigned epochs);
        void trainJNet(unsigned nInputs, unsigned nOutputs, unsigned epochs);
        NeuralNetwork* getNN() { return this->net; }
        JNet* getJNet() { return this->jnet; }
        void setFilename(string fn) { this->filename = fn; }
        vector<double> getPrediction(vector<double> inputs);
        vector<double> getPredictionJNet(vector<double> inputs);

    private:
        NeuralNetwork* net;
        JNet* jnet;
        string filename;
        vector<string> getNextLineAndSplitIntoTokens(istream& str);
};

#endif
