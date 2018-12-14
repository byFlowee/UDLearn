#include "JNet.h"
using namespace std;

double JNet::smoothingFactor = 100;

JNet::JNet(const vector<unsigned> &topology) {
    srand ( time(NULL) );
    unsigned numLayers = topology.size();
    for (unsigned layerId = 0; layerId < numLayers;  ++layerId) {
        unsigned nOutputs = (layerId == topology.size() - 1) ? 0 : topology[layerId + 1];
        vector<Perceptron> newLayer;
        for (unsigned neuronId = 0; neuronId <= topology[layerId]; ++neuronId) {
            newLayer.push_back(Perceptron(nOutputs, neuronId));
        }
        newLayer.back().setOutput(1.0); //set bias to 1
        layers.push_back(newLayer);
        newLayer.clear();
    }
}

void JNet::getResults(vector<double> &results) const {
    results.clear();

    for (unsigned p = 0; p < layers.back().size() - 1; ++p) {
        results.push_back(layers.back()[p].getOutput());
    }
}

void JNet::feedForward(const vector<double> &input) {
    
    //Feed the first layer with the input values
    for (unsigned inputId = 0; inputId < input.size(); ++inputId) {
        layers[0][inputId].setOutput(input[inputId]);
    }

    for (unsigned layerId = 1; layerId < layers.size(); ++layerId) {
        vector<Perceptron> &prev = layers[layerId - 1];
        for (unsigned neuronId = 0; neuronId < layers[layerId].size() - 1; ++neuronId) {
            layers[layerId][neuronId].computeOutput(prev);
        }
    }
}

void JNet::backPropagation(const vector<double> &target) {
    vector<Perceptron> &outputLayer = layers.back();
    loss = 0.0;

    for (unsigned p = 0; p < outputLayer.size() - 1; ++p) {
        double d = target[p] - outputLayer[p].getOutput();
        loss += d * d;
    }

    loss /= (outputLayer.size() - 1);
    loss = sqrt(loss); // - 1 to exclude the bias

    avgError =
            (avgError * smoothingFactor + loss)
            / (smoothingFactor + 1.0);

    //Gradient for output layer
    for (unsigned p = 0; p < outputLayer.size() -1; ++p) {
        outputLayer[p].computeOutputGradients(target[p]);
    }

    //Gradient for hidden layers, iterate backwards
    for (unsigned layerId = layers.size() - 2; layerId > 0; --layerId) {
         for (unsigned p = 0; p < layers[layerId].size(); ++p) {
             layers[layerId][p].computeHiddenGradients(layers[layerId + 1]);
         }
    }

    //Update weights
    for (unsigned layerId = layers.size() - 1; layerId > 0; --layerId) {
        for (unsigned p = 0; p < layers[layerId].size(); ++p) {
            layers[layerId][p].updateWeights(layers[layerId - 1]);
        }
    } 
}

void JNet::printTopology() const {
    cout << "Topology:" << endl;
    
    for (int i = 0; i < layers.size(); ++i) {
        cout << "Layer " << i << ": " << layers[i].size() << " perceptrons" << endl;
    }

}