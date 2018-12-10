#include "Perceptron.h"
#include <iostream>

Perceptron::Perceptron(unsigned inputSize) { 
    for (unsigned i = 0; i < inputSize; ++i) 
        this->weights.push_back(Perceptron::getRandomWeight());  
}

void Perceptron::computeOutput() {
    double sum = 0.0;
    for (unsigned i = 0; i < this->inputs.size(); ++i) {
        sum += this->weights[i] * this->inputs[i];
    }

    this->output = sum;
}

//Input values are set in this->inputs
void Perceptron::updateWeights(double target) {     
    
    for (unsigned i = 0; i < weights.size(); ++i) {
        this->weights[i] += target * this->inputs[i];
    }
}

double Perceptron::getPrediction() const {
    return (this->output < 0.0) ? -1.0 : 1.0;
}

void Perceptron::trainPerceptron(unsigned epochs, const vector<vector<double> > &inputs, const vector<double> &targets) {

    for (unsigned e = 0; e < epochs; ++e) {
        vector<double> predictions; 
        double pred;
        unsigned sum = 0;

        for(unsigned d = 0; d < inputs.size(); ++d) {
            cout << weights[0] << " " << weights[1];
            this->inputs = inputs[d];
            this->computeOutput();
            pred = this->getPrediction();   // returns -1 or 1
            predictions.push_back(pred);
            
            if (pred < 0) 
                sum++;
        }

        if (sum == 0) {
            cout << "100% accuracy on training set" << endl;
            return;
        }
        else {
            int random = rand() % sum;
            int aux = 0;

            for (unsigned i = 0; i < predictions.size(); ++i) {
                if (predictions[i] < 0) {
                    if(aux == random) {
                        this->inputs = inputs[i];
                        this->updateWeights(targets[i]);
                        break;
                    }

                    aux++;
                }
            }
        }

        for (unsigned i = 0; i < predictions.size(); ++i) {
            cout << predictions[i] << " ";
        }
        cout << endl;
    }
}

int main() {
    Perceptron p(2);

    vector<vector<double>> x;
    vector<double> aux;
    aux.push_back(0.0);
    aux.push_back(0.0);
    x.push_back(aux);
    
    aux.clear();

    aux.push_back(0.0);
    aux.push_back(1.0);
    x.push_back(aux);

    aux.clear();

    aux.push_back(1.0);
    aux.push_back(0.0);
    x.push_back(aux);

    aux.clear();

    aux.push_back(1.0);
    aux.push_back(1.0);
    x.push_back(aux);

    vector<double> targets;
    targets.push_back(-1.0);
    targets.push_back(-1.0);
    targets.push_back(-1.0);
    targets.push_back(1.0);

    p.trainPerceptron(20, x, targets);
}
