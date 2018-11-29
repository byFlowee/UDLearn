#include "Perceptron.h"

double Perceptron::lr = 0.2;
double Perceptron::momentum = 0.5;

Perceptron::Perceptron(unsigned nOutputs, unsigned index) {
    
    for (unsigned i = 0; i < nOutputs; ++i) {
        weights.push_back(randomWeightInit());
        deltaWeights.push_back(randomWeightInit());
    }

    this->index = index;
}

void Perceptron::updateWeights(vector<Perceptron> &previousLayer) {
    for (unsigned p = 0; p < previousLayer.size(); ++p) {
        double previousDeltaWeight = previousLayer[p].deltaWeights[index];
        double deltaWeight = lr * previousLayer[p].getOutput() * this->gradient + momentum * previousDeltaWeight;

        previousLayer[p].deltaWeights[index] = deltaWeight;
        previousLayer[p].weights[index] += deltaWeight;
    }
}

//dE/dyⱼ = Σ ∂E/∂zⱼ * ∂z/∂yⱼ = Σ ∂E/∂zⱼ * wᵢⱼ
void Perceptron::computeHiddenGradients(vector<Perceptron> &nextLayer) {

     double loss = 0.0;

     for (unsigned p = 0; p < nextLayer.size() - 1; ++p) {
         loss += weights[p] * nextLayer[p].gradient;
     } 

    this->gradient = loss * Perceptron::activationFunctionDerivate(output);
}

void Perceptron::computeOutputGradients(double target) {
    double delta = target - this->output;
    this->gradient = delta * Perceptron::activationFunctionDerivate(output);
}

void Perceptron::computeOutput(const vector<Perceptron> &prev) {
    double sum = 0;
    for (unsigned p = 0; p < prev.size(); ++p) {
        sum += prev[p].getOutput() * prev[p].weights[index];
    }

    this->output = Perceptron::activationFunction(sum);
}

double Perceptron::activationFunction(double x) {
    return tanh(x);
}

double Perceptron::activationFunctionDerivate(double x) {
    double t = tanh(x);
    return 1.0 - t*t;
}