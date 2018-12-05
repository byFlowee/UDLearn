#include "DataLoader.h"

DataLoader::DataLoader(string filename, vector<int> &topology) {
    net = new NeuralNetwork(topology);
    this->filename = filename;
}

vector<string> DataLoader::getNextLineAndSplitIntoTokens(istream& str) {
    vector<string> result;
    string line;
    getline(str,line);

    stringstream lineStream(line);
    string cell;

    while(getline(lineStream,cell, ',')) {
        result.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty()) {
        // If there was a trailing comma then add an empty element.
        result.push_back("");
    }
    
    return result;
}

void DataLoader::trainNN(unsigned nInputs, unsigned nOutputs, unsigned epochs) {
    ifstream file;
    file.open(this->filename);

    if(file.is_open()) {
        vector<string> line;
        string dataTrainingLabels;

        getline(file, dataTrainingLabels);
        cout << dataTrainingLabels << endl;
    
        vector<Mat> inputs;
        vector<Mat> expectedOutputs;

        bool eof = false;
        line = getNextLineAndSplitIntoTokens(file);

        while(!eof) {            
            Mat inputs1(1, nInputs);
            Mat expectedOutputs1(1, nOutputs);

            for (unsigned i = 0; i < nInputs; ++i) {
                inputs1.set(0, i, atof(line[i].c_str()));
            }
            
            for (unsigned o = 0; o < nOutputs; ++o) {
                expectedOutputs1.set(0, o, atof(line[nInputs + o].c_str()));
            }
            
            inputs.push_back(inputs1);
            expectedOutputs.push_back(expectedOutputs1);

            line.clear();
            line = getNextLineAndSplitIntoTokens(file);

            if (line.size() < (nInputs + nOutputs)) 
                eof = true;
        }

        cout << "Training!" << endl;
        this->net->train(inputs, expectedOutputs, epochs);
    }
    else {
        cout << "Error: File \"" << this->filename << "\" couldn't be opened" << endl;
    }
}