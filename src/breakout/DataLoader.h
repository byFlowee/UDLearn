#ifndef _DATALOADER_H_
#define _DATALOADER_H_

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "../NeuralNetwork/mat.h"
#include "../NeuralNetwork/neuralNetwork.h"

using namespace std;

class DataLoader {
    public:
        DataLoader(string filename, vector<int> &topology);
        ~DataLoader() { delete this->net; this->net = NULL; }
        void trainNN(unsigned nInputs, unsigned nOutputs, unsigned epochs);
        NeuralNetwork* getNN() { return this->net; }
        void setFilename(string fn) { this->filename = fn; }
        vector<double> getPrediction(vector<double> inputs);

    private:
        NeuralNetwork* net;
        string filename;
        vector<string> getNextLineAndSplitIntoTokens(istream& str);
};

#endif
