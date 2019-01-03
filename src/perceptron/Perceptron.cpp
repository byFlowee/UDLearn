#include "Perceptron.h"
#include <iostream>
#include <time.h>
#include <limits>

Perceptron::Perceptron(unsigned inputSize) {
    srand(time(NULL));
    for (unsigned i = 0; i <= inputSize; ++i) 
        this->weights.push_back(Perceptron::getRandomWeight());  
}

double Perceptron::computeOutput() {
    double sum = 0.0;
    for (unsigned i = 0; i < this->inputs.size(); ++i) {
        sum += this->weights[i] * this->inputs[i];
    }

    return (sum < 0.0) ? -1.0 : 1.0;
}

//Input values are set in this->inputs
void Perceptron::updateWeights(double target) {

    for (size_t w = 0; w < weights.size(); ++w)
        this->weights[w] += (target * this->inputs[w]);
}

double Perceptron::getPrediction(const vector<double> &inputs) {
    this->setInputValues(inputs);
    return this->computeOutput();   
}

void Perceptron::setInputValues(const vector<double> &inputs) {
    this->inputs = inputs;
    this->inputs.insert(this->inputs.begin(), 1.0);
}

void Perceptron::trainPerceptron(unsigned epochs, const vector<vector<double> > &inputs, const vector<double> &targets) {
    this->pocket.clear();
    bestSolution = numeric_limits<unsigned>::max();
    
    for (unsigned e = 0; e < epochs; ++e) {
        vector<double> predictions; 
        vector<unsigned> missclassifiedPoints;
        double pred;
        unsigned sum = 0;

        cout << "Weights: ";

        for (size_t i = 0; i < this->weights.size(); i++)
        {
            cout << this->weights[i];

            if (i + 1 != this->weights.size())
            {
                cout << ", ";
            }
            else
            {
                cout << endl;
            }
        }

        for(unsigned d = 0; d < inputs.size(); ++d) {
            this->setInputValues(inputs[d]);
            pred = this->computeOutput();
            predictions.push_back(pred);
            
            if (pred + targets[d] < 0.1 && pred + targets[d] > -0.1)
            {
                // If prediction + expectedResult == 0 then it's a missclassified point (they are not equal (1 + (-1) or (-1) + 1))
                missclassifiedPoints.push_back(d);
            }
        }

        if (bestSolution > missclassifiedPoints.size()) {
            this->bestSolution = missclassifiedPoints.size();
            this->pocket = this->weights;
        }

        if (missclassifiedPoints.size() == 0) {
            cout << "100% accuracy on training set!" << endl;
            return;
        }
        else {
            unsigned random = rand() % missclassifiedPoints.size();
            
            for (unsigned i = 0; i < missclassifiedPoints.size(); ++i) {
                if(i == random) {
                    cout << "Updating weights" << endl;
                    this->setInputValues(inputs[missclassifiedPoints[i]]);
                    this->updateWeights(targets[missclassifiedPoints[i]]);
                    break;
                }
            }
        }

        cout << "Predictions: ";

        for (unsigned i = 0; i < predictions.size(); ++i) {
            cout << predictions[i];

            if (i + 1 != predictions.size())
            {
                cout << " ";
            }
            else
            {
                cout << endl;
            }
        }

        cout << endl;
    }

    if (!this->pocket.empty()) {
        this->weights = this->pocket;
    } 
}
