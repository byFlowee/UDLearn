#include "DataLoader.h"

DataLoader::DataLoader(string filename, vector<int> &topology) {
    vector<unsigned> jTopology(topology.begin(), topology.end());
    net = new NeuralNetwork(topology);
    jnet = new JNet(jTopology);
    this->filename = filename;
}
DataLoader::~DataLoader(){
    delete this->net;
    delete this->jnet;
    this->net = NULL;
    this->jnet = NULL;
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

vector<double> DataLoader::getPrediction(vector<double> inputs) {
    vector<double> outputs;
    Mat values(1, inputs.size());
    for (unsigned i = 0; i < inputs.size(); ++i)
        values.set(0, i, inputs[i]);
    
    Mat result = this->net->forwardPropagation(values);

    for (int i = 0; i < inputs.size(); ++i)
        outputs.push_back(result.get(0, i));

    return outputs;    
}

vector<double> DataLoader::getPredictionJNet(vector<double> inputs) {
    vector<double> outputs;

    this->jnet->feedForward(inputs);
    this->jnet->getResults(outputs);

    return outputs;
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

void DataLoader::trainJNet(unsigned nInputs, unsigned nOutputs, unsigned epochs) {
    ifstream file;
    file.open(this->filename);

    if(file.is_open()) {
        vector<string> line;
        string dataTrainingLabels;

        getline(file, dataTrainingLabels);
        cout << dataTrainingLabels << endl;
    
        vector<vector<double>> inputs;
        vector<vector<double>> expectedOutputs;

        bool eof = false;
        line = getNextLineAndSplitIntoTokens(file);

        while(!eof) {            
            vector<double> inputs1;                      //Mat inputs1(1, nInputs);
            vector<double> expectedOutputs1;             //Mat expectedOutputs1(1, nOutputs);

            for (unsigned i = 0; i < nInputs; ++i) {
                inputs1.push_back(atof(line[i].c_str()));
            }
            
            for (unsigned o = 0; o < nOutputs; ++o) {
                expectedOutputs1.push_back(atof(line[nInputs + o].c_str()));
            }
            
            inputs.push_back(inputs1);
            expectedOutputs.push_back(expectedOutputs1);

            line.clear();
            line = getNextLineAndSplitIntoTokens(file);

            if (line.size() < (nInputs + nOutputs)) 
                eof = true;
        }

        for(vector<double> a : inputs) {
            cout << a.size() << endl;
        }

        cout << "Training!" << endl;

        for (unsigned e = 0; e < epochs; ++e) {
            for (unsigned i = 0; i < inputs.size(); ++i) {
                this->jnet->feedForward(inputs[i]);
                this->jnet->backPropagation(expectedOutputs[i]);
            }
        }
        
        file.close();
    }
    else {
        cout << "Error: File \"" << this->filename << "\" couldn't be opened" << endl;
    }
}