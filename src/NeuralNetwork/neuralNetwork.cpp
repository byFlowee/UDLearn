
#include <iostream>
#include <cmath>

#include "neuralNetwork.h"

NeuralNetwork::NeuralNetwork(const vector<int> &size) :
    size(size),
    dropout(size.size(), 0.0),
    learningRate(0.1),
    error(1, size[size.size() - 1])
{
    this->initialize();
}

NeuralNetwork::NeuralNetwork(const vector<int> &size, const vector<double> &dropout) :
    error(1, size[size.size() - 1])
{
    //last dropout value corresponding to output layer wont have any effect
    if (dropout.size() == size.size()) {
        this->dropout = dropout;
        this->size = size;
        this->learningRate = 0.1;

        this->initialize();
    } else {
        cerr << "ERROR: dropout must have the same topology as size, couldn't build NeuralNetwork." << endl;
        throw;
    }
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

void NeuralNetwork::updateDropoutMats() {
    this->dropoutMats.clear();
    for (size_t l = 0; l < this->size.size(); ++l) {
        Mat dropout(1, this->size[l], 1);
        if (this->dropout[l] > 0.0) {
            for (int n = 0; n < this->size[l]; ++n) {
                double randomDist = ((double)rand() / (double)RAND_MAX);
                dropout.set(0, n, (randomDist < this->dropout[l]) ? 0 : 1);
            }
        }

        //dropout.print();
        //cout << endl;
        this->dropoutMats.push_back(dropout);
    }
}

Mat NeuralNetwork::forwardPropagation(const Mat &initial, bool training)
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

        //--DROPOUT
        if (training && this->dropout[i+1] > 0.0) {
            this->a.back() = this->a.back().directMult(this->dropoutMats[i+1]);
            this->da.back() = this->da.back().directMult(this->dropoutMats[i+1]);
        }
        //--DROPOUT
    }

    res = outputs;

    return res;
}

void NeuralNetwork::backPropagation(const Mat &inputs, const Mat &expectedOutputs)
{
    Mat outputs = this->forwardPropagation(inputs, true);
    Mat error = expectedOutputs.sub(outputs);
    Mat derror = expectedOutputs.sub(outputs);

    for (int i = 0; i < error.size(); i++)
    {
        error.set(0, i, error.get(0, i) * error.get(0, i));
    }

    error.scalar(0.5);
    derror.scalar(-1.0);

    this->error = error;

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
            double value = this->da[i + 1].get(0, j) * this->dropoutMats[i+1].get(0, j); //multiplied by 0 if the neuron is disabled so the delta is not considered

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

    cout << "Training!" << endl;
    for (int i = 0; i < iterations; i++)
    {
        double totalError = 0;
        //Para cada iteración cambiamos las neuronas marcadas como inactivas
        this->updateDropoutMats();
        for (size_t j = 0; j < inputs.size(); j++)
        {
            this->backPropagation(inputs[j], expectedOutputs[j]);

            double meanError = 0;
            for (int k = 0; k < this->error.size(); ++k) 
                meanError += this->error.get(0, k);

            totalError += (meanError / this->error.size());
        }

        cout << "Epoch " << i << "\n Error: " << totalError / inputs.size() << endl;
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

vector<string> NeuralNetwork::getNextLineAndSplitIntoTokens(istream& str)
{
    vector<string> result;
    string line;
    getline(str,line);

    stringstream lineStream(line);
    string cell;

    while(getline(lineStream,cell, ','))
    {
        result.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        result.push_back("");
    }
    return result;
}

double NeuralNetwork::getTotalError(const vector<Mat> &inputs, const vector<Mat> &expectedOutputs) {

    double current = 0;
    double total = 0;

    for (size_t d = 0; d < inputs.size(); ++d) {
        Mat outputs = this->forwardPropagation(inputs[d], false);
        Mat error = expectedOutputs[d].sub(outputs);
        
        for (int i = 0; i < error.size(); i++) {
            error.set(0, i, error.get(0, i) * error.get(0, i));
        }

        error.scalar(0.5);

        for (int i = 0; i < error.size(); ++i) {
            current += error.get(0, i);
        }

        current /= error.size();    //mean erro on one sample
        total += current;
    }

    return (total / inputs.size());
}

void NeuralNetwork::crossFoldValidation(unsigned epochs, unsigned folds, string filename) {
    ifstream file;
    file.open(filename);

    if(file.is_open()) {
        unsigned nInputs = this->size.front();
        unsigned nOutputs = this->size.back();
        vector<string> line;
        string dataTrainingLabels;

        getline(file, dataTrainingLabels);
        cout << dataTrainingLabels << endl;
    
        vector<Mat> inputs;
        vector<Mat> expectedOutputs;

        bool eof = false;
        unsigned totalDataInputs = 0;
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
            
            totalDataInputs++;

            if (line.size() < (nInputs + nOutputs)) 
                eof = true;
        }

        unsigned perFoldSamples = totalDataInputs / folds;
        unsigned currentSamples;

        double totalFoldScore = 0;
        double currentFoldScore = 0;

        vector<Mat> validationInputs;
        vector<Mat> validationOutputs;

        vector<Mat> trainingInputs;
        vector<Mat> trainingOutputs;

        //CROSSFOLD VALIDATION
        for (size_t f = 0; f < folds; ++f) {
            cout << "Fold [" << f + 1 << "/" << folds << "]" << endl;
            currentSamples = f * perFoldSamples;            
            
            //DATA SHUFFLING
            for (size_t d = 0; d < totalDataInputs; ++d) {
                if (d >= currentSamples && d < currentSamples + perFoldSamples) {
                    validationInputs.push_back(inputs[d]);
                    validationOutputs.push_back(expectedOutputs[d]);
                } else {
                    trainingInputs.push_back(inputs[d]);
                    trainingOutputs.push_back(expectedOutputs[d]);
                }
            }

            NeuralNetwork net(this->size, this->dropout);
            net.train(trainingInputs, trainingOutputs, epochs);
            
            currentFoldScore = net.getTotalError(validationInputs, validationOutputs);
            totalFoldScore += currentFoldScore;

            validationInputs.clear();
            validationOutputs.clear();
            
            trainingInputs.clear();
            trainingOutputs.clear();

            cout << "Fold total Score on validation set: " << currentFoldScore << endl;
        }

        cout << "CrossValidation mean score: " << totalFoldScore / folds << endl;
    }
    else {
        cerr << "File \"" << filename << "\" couldn't be oppened" << endl;
    }
}

vector<Mat> NeuralNetwork::getWeights() const
{
    return this->weights;
}

vector<Mat> NeuralNetwork::getBias() const
{
    return this->bias;
}

void NeuralNetwork::setWeights(const vector<Mat> &weights)
{
    this->weights = weights;
}

void NeuralNetwork::setBias(const vector<Mat> &bias)
{
    this->bias = bias;
}

vector<int> NeuralNetwork::getTopology() const
{
    return this->size;
}