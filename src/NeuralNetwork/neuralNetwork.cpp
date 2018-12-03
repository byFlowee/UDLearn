
#include <iostream>
#include <cmath>

#include "neuralNetwork.h"

NeuralNetwork::NeuralNetwork(const vector<int> &size)
{
    this->size = size;
    this->learningRate = 0.1;
    
    this->initialize();
}

void NeuralNetwork::initialize()
{
    srand(time(NULL));

    for (size_t i = 0; i < this->size.size() - 1; i++)
    {
        Mat matWeights(this->size[i], this->size[i + 1]);
        Mat matBias(1, this->size[i + 1]);  // Vector (row mat).

        this->weights.push_back(matWeights);
        this->bias.push_back(matBias);
        
        // Random values initialization.
        for (int row = 0; row < this->weights[i].rows(); row++)
        {
            for (int col = 0; col < this->weights[i].cols(); col++)
            {
                double randomValueW = -1.0 + ((double)rand() / RAND_MAX) * 2.0;
                double randomValueB = -1.0 + ((double)rand() / RAND_MAX) * 2.0;

                this->weights[i].set(row, col, randomValueW);
                this->bias[i].set(0, col, randomValueB);		// Same col size.
            }
        }
    }
}

void NeuralNetwork::setLearningRate(double learningRate)
{
    this->learningRate = learningRate;
}

double NeuralNetwork::sigmoid(double d) const
{
    return 1.0 / (1.0 + pow(exp(1), (-1.0) * d));
}

double NeuralNetwork::sigmoidPrime(double d) const
{
    return this->sigmoid(d) * (1.0 - this->sigmoid(d));
}

double NeuralNetwork::activationFunction(double d) const
{
    return this->sigmoid(d);
}

double NeuralNetwork::activationFunctionPrime(double d) const
{
    return this->sigmoidPrime(d);
}

Mat NeuralNetwork::forwardPropagation(const Mat &initial)
{
    Mat res(1, this->size[this->size.size() - 1]);
    Mat outputs = initial.copy();   // In first layer input = output (we do not apply activation function).

    this->a.clear();
    this->da.clear();

    this->a.push_back(initial.copy());
    this->da.push_back(initial.copy());
    
    for (size_t i = 0; i < this->size.size() - 1; i++)
    {
        outputs = outputs.mult(this->weights[i]);
        
        this->a.push_back(outputs.copy());
        
        for (int j = 0; j < outputs.size(); j++)
        {
            outputs.set(0, j, this->activationFunction(outputs.get(0, j) + this->bias[i].get(0, j)));
        }
        
        this->da.push_back(outputs.copy());
    }
    
    res = outputs;
    
    return res;
}

void NeuralNetwork::backPropagation(const Mat &inputs, const Mat &expectedOutputs)
{
    Mat outputs = this->forwardPropagation(inputs);
    Mat error = expectedOutputs.sub(outputs);
    Mat derror = expectedOutputs.sub(outputs);
    
    for (int i = 0; i < error.size(); i++)
    {
        error.set(0, i, error.get(0, i) * error.get(0, i));
    }
    
    error.scalar(0.5);
    derror.scalar(-1.0);
    
    //ArrayList<Mat> delta = new ArrayList<>();
    vector<Mat> delta;
    
    for (size_t i = 1; i < this->size.size(); i++)	// delta0 is not useful.
    {
        Mat currentDelta(1, this->size[i]);

        delta.push_back(currentDelta);
    }

    // First add manually delta from last layer.
    for (int i = 0; i < delta[delta.size() - 1].size(); i++)
    {
        double value = this->activationFunctionPrime(outputs.get(0, i)) * derror.get(0, i);
        
        delta[delta.size() - 1].set(0, i, value);
    }

    // Rest of deltas.
    for (int i = delta.size() - 2; i >= 0; i--)
    {
        // delta i.
        for (int j = 0; j < delta[i].size(); j++)
        {
            //Double value = this.da.get(i + 1).get(0, j);
            double value = this->da[i + 1].get(0, j);
            
            for (int k = 0; k < delta[i + 1].size(); k++)
            {
                //value *= this.weights.get(i + 1).get(j, k) * delta.get(i + 1).get(0, k);
                value *= this->weights[i + 1].get(j, k) * delta[i + 1].get(0, k);
            }
            
            //delta.get(i).set(0, j, value);
            delta[i].set(0, j, value);
        }
    }
    
    // Weights update.
    for (size_t i = 0; i < this->weights.size(); i++)
    {
        // Layer i.
        Mat &weight = this->weights[i];
        
        for (int row = 0; row < this->weights[i].rows(); row++)
        {
            for (int col = 0; col < this->weights[i].cols(); col++)
            {
                //weight.set(row, col, weight.get(row, col) - this.learningRate * this.a.get(i).get(0, row) * delta.get(i).get(0, col));
                weight.set(row, col, weight.get(row, col) - this->learningRate * this->a[i].get(0, row) * delta[i].get(0, col));
            }
        }
    }
    
    // Bias update.
    for (size_t i = 0; i < this->weights.size(); i++)
    {
        // Layer i.
        Mat &bias = this->bias[i];
        
        for (int row = 0; row < this->bias[i].rows(); row++)
        {
            //bias.set(0, row, bias.get(0, row) - this.learningRate * delta.get(i).get(0, row));
            bias.set(0, row, bias.get(0, row) - this->learningRate * delta[i].get(0, row));
        }
    }
}

void NeuralNetwork::train(const vector<Mat> &inputs, const vector<Mat> &expectedOutputs, int iterations)
{
    if (inputs.size() != expectedOutputs.size())
    {
        return;
    }
    
    for (int i = 0; i < iterations; i++)
    {
        for (size_t j = 0; j < inputs.size(); j++)
        {
            this->backPropagation(inputs[j], expectedOutputs[j]);
        }
    }
}

string NeuralNetwork::description() const
{
    string res = "";
    int max = 0;
    
    for (size_t i = 0; i < this->size.size(); i++)
    {
        if (this->size[i] > max)
        {
            max = this->size[i];
        }
    }
    
    for (int i = 0; i < max; i++)
    {
        for (size_t j = 0; j < this->size.size(); j++)
        {
            if (i < this->size[j])
            {
                res += "* ";
            }
            else
            {
                res += "  ";
            }
        }
        
        res += "\n";
    }
    
    return res;
}