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

void Perceptron::trainPerceptron(unsigned epochs, const vector<vector<double> > &inputs, const vector<double> &targets, bool verbose) {
    this->pocket.clear();
    this->bestSolution = numeric_limits<unsigned>::max();
    double lastAccuracy = numeric_limits<double>::lowest();
    double lastError = numeric_limits<double>::max();

    
    for (unsigned e = 0; e < epochs; ++e) {
        vector<double> predictions; 
        vector<unsigned> misclassifiedPoints;
        double pred;

        if (verbose)
        {
            cout << "Epoch " << (e + 1) << endl;
            cout << "---------------------------------------" << endl;
            cout << "  Weights: ";

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
        }

        for(unsigned d = 0; d < inputs.size(); ++d) {
            this->setInputValues(inputs[d]);
            pred = this->computeOutput();
            predictions.push_back(pred);
            
            if (pred + targets[d] < 0.1 && pred + targets[d] > -0.1)
            {
                // If prediction + expectedResult == 0 then it's a missclassified point (they are not equal (1 + (-1) or (-1) + 1))
                misclassifiedPoints.push_back(d);
            }
        }

        double accuracy = ((double)(inputs.size() - misclassifiedPoints.size()) / (double)inputs.size());
        double error = 1.0 - accuracy;

        if (verbose)
        {
            if (accuracy > lastAccuracy)
            {
                cout << "  Accuracy: \033[1;32m" << accuracy << "\033[0m (\033[1;32m" << accuracy * 100.0 << " %\033[0m)" << endl;
            }
            else
            {
                cout << "  Accuracy: \033[1;31m" << accuracy << "\033[0m (\033[1;31m" << accuracy * 100.0 << " %\033[0m)" << endl;
            }

            if (error < lastError)
            {
                cout << "  Error: \033[1;32m" << error << "\033[0m (\033[1;32m" << error * 100.0 << " %\033[0m)" << endl;
            }
            else
            {
                cout << "  Error: \033[1;31m" << error << "\033[0m (\033[1;31m" << error * 100.0 << " %\033[0m)" << endl;
            }
        }

        if (bestSolution > misclassifiedPoints.size()) {
            this->bestSolution = misclassifiedPoints.size();
            this->pocket = this->weights;
        }

        if (misclassifiedPoints.size() == 0) {
            break;
        }
        else {
            unsigned random = rand() % misclassifiedPoints.size();
            
            for (unsigned i = 0; i < misclassifiedPoints.size(); ++i) {
                if(i == random) {
                    this->setInputValues(inputs[misclassifiedPoints[i]]);
                    this->updateWeights(targets[misclassifiedPoints[i]]);
                    break;
                }
            }
        }

        if (verbose)
        {
            cout << "  Predictions: ";

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

        lastAccuracy = accuracy;
        lastError = error;
    }

    if (!this->pocket.empty()) {
        this->weights = this->pocket;
    }

    if (verbose)
    {
        cout << endl;
    }
    
    cout << "Best weights found: ";

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

    cout << "Best accuracy: " << ((double)(inputs.size() - this->bestSolution) / (double)inputs.size()) * 100.0 << "%" << endl;
    cout << "Best error: " << (1.0 - ((double)(inputs.size() - this->bestSolution) / (double)inputs.size())) * 100.0 << "%" << endl;
    
}
