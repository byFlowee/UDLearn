#include "Network.h"
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>


class TrainingData
{
public:
    TrainingData(const string filename);
    bool isEof(void) { return m_trainingDataFile.eof(); }
    void getTopology(vector<unsigned> &topology);

    // Returns the number of input values read from the file:
    unsigned getNextInputs(vector<double> &inputVals);
    unsigned getTargetOutputs(vector<double> &targetOutputVals);

private:
    ifstream m_trainingDataFile;
};

void TrainingData::getTopology(vector<unsigned> &topology)
{
    string line;
    string label;

    getline(m_trainingDataFile, line);
    stringstream ss(line);
    ss >> label;
    if (this->isEof() || label.compare("topology:") != 0) {
        abort();
    }

    while (!ss.eof()) {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }

    return;
}

TrainingData::TrainingData(const string filename)
{
    m_trainingDataFile.open(filename.c_str());
}

unsigned TrainingData::getNextInputs(vector<double> &inputVals)
{
    inputVals.clear();

    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);

    string label;
    ss>> label;
    if (label.compare("in:") == 0) {
        double oneValue;
        while (ss >> oneValue) {
            inputVals.push_back(oneValue);
        }
    }

    return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(vector<double> &targetOutputVals)
{
    targetOutputVals.clear();

    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);

    string label;
    ss>> label;
    if (label.compare("out:") == 0) {
        double oneValue;
        while (ss >> oneValue) {
            targetOutputVals.push_back(oneValue);
        }
    }

    return targetOutputVals.size();
}

void showVectorVals(string label, vector<double> &v)
{
    cout << label << " ";
    for (unsigned i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }

    cout << endl;
}

int main() {
    // vector<unsigned> topology;
    // topology.push_back(2);
    // topology.push_back(2);
    // topology.push_back(1);
    
    // Network n(topology);

    // n.printTopology();

    // vector< vector<double> > trainingInputs;
    // vector<double> trainingTargets;

    // string filename = "xor.txt";
    // ifstream data;
    // data.open(filename);
    // if(data.is_open()) {
    //     string line;
    //     vector<double> input;
    //     while(getline(data,line)) {
    //         input.clear();
    //         input.push_back(double(line[0]-'0'));
    //         input.push_back(double(line[2]-'0'));
    //         trainingInputs.push_back(input);
    //         trainingTargets.push_back(double(line[4]-'0'));
    //     }
    // }
    // else {
    //     cout << "File " << filename << " not found" << endl;
    // }

    // vector<double> results;
    // for (unsigned epochs = 0; epochs < trainingInputs.size(); ++epochs) {
    //     n.feedForward(trainingInputs[epochs]);
    //     n.getResults(results);
    //     n.backPropagation(trainingTargets);

        
    //     cout << "Epoch " << epochs << ": " << endl;
    //     cout << "\t" << trainingInputs[epochs][0] << " " << trainingInputs[epochs][1] << " " << trainingTargets[epochs] << " | " << results[0] << endl;
    // }

    
    // n.getResults(results);
    

    // /*
    // for (int i = 0; i < trainingInputs.size(); ++i)
    //     cout << trainingInputs[i][0] << " " << trainingInputs[i][1] << endl;

    // for (int i = 0; i < trainingTargets.size(); ++i)
    //     cout << trainingTargets[i] << endl;
    // */


    
    // return 0;

       TrainingData trainData("notepad.txt");

    // e.g., { 3, 2, 1 }
    vector<unsigned> topology;
    trainData.getTopology(topology);

    Network myNet(topology);

    vector<double> inputVals, targetVals, resultVals;
    int trainingPass = 0;

    while (!trainData.isEof()) {
        ++trainingPass;
        cout << endl << "Pass " << trainingPass;

        // Get new input data and feed it forward:
        if (trainData.getNextInputs(inputVals) != topology[0]) {
            break;
        }
        showVectorVals(": Inputs:", inputVals);
        myNet.feedForward(inputVals);

        // Collect the net's actual output results:
        myNet.getResults(resultVals);
        showVectorVals("Outputs:", resultVals);

        // Train the net what the outputs should have been:
        trainData.getTargetOutputs(targetVals);
        showVectorVals("Targets:", targetVals);
        assert(targetVals.size() == topology.back());

        myNet.backPropagation(targetVals);

        // Report how well the training is working, average over recent samples:
        cout << "Net recent average error: "
                << myNet.getRecentAverageError() << endl;
    }

    cout << endl << "Done" << endl;

}

// < <1 1> <1 0> <0 1> >