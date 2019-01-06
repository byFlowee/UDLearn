
#include <iostream>
#include <vector>
#include <fstream>

#include "Perceptron.h"

using namespace std;

void ANDFunction()
{
    string filename = "ANDFunction";
    Perceptron p(2);

    int epochs = 20;
    vector<vector<double>> inputs;
    vector<double> targets = {-1.0, -1.0, -1.0, 1.0};

    cout << "Epochs: " << epochs << endl;
    cout << endl;

    inputs.push_back({0.0, 0.0});
    inputs.push_back({0.0, 1.0});
    inputs.push_back({1.0, 0.0});
    inputs.push_back({1.0, 1.0});

    p.trainPerceptron(epochs, inputs, targets);

    cout << endl;
    cout << "A B : A^B" << endl;
    cout << "---------" << endl;
    cout << "0 0 :  " << p.getPrediction({0.0, 0.0}) << endl;
    cout << "0 1 :  " << p.getPrediction({0.0, 1.0}) << endl;
    cout << "1 0 :  " << p.getPrediction({1.0, 0.0}) << endl;
    cout << "1 1 :  " << p.getPrediction({1.0, 1.0}) << endl;

    ofstream weigthsOutputs((filename + ".weights").c_str());

    if (!weigthsOutputs.is_open())
    {
        cerr << "ERROR: could't open the file \"" << (filename + ".weights") << "\"." << endl;
    }
    else
    {
        weigthsOutputs << "Accuracy: " << p.getBestAcc() * 100.0 << "%" << endl;
        weigthsOutputs << "Error: " << (1.0 - p.getBestAcc()) * 100.0 << "%" << endl;
        weigthsOutputs << endl;

        vector<double> weights = p.getWeights();

        for (size_t i = 0; i < weights.size(); i++)
        {
            weigthsOutputs << weights[i];

            if (i + 1 != weights.size())
            {
                weigthsOutputs << ",";
            }
        }
    }
}

double getRandomDouble(double min, double max)
{
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

void functions(int function, int points, int epochs, bool verbose = false)
{
    cout << "Points: " << points << endl;
    cout << "Epochs: " << epochs << endl;
    cout << "Verbose: " << (verbose ? "yes" : "no") << endl;
    cout << endl;

    string filename = "function" + to_string(function) + ".csv";
    Perceptron p(2);
    vector<vector<double>> inputs;
    vector<double> targets;
    ofstream file(filename.c_str());

    if (!file.is_open())
    {
        cerr << "ERROR: could't open the file \"" << filename << "\"." << endl;
    }
    else
    {
        file << "x,y,target" << endl;
    }

    for (int i = 0; i < points; i++)
    {
        double x = getRandomDouble(0.0, 1.0);
        double y = getRandomDouble(0.0, 1.0);
        double target = 0.0;
        bool change = true;

        inputs.push_back({x, y});

        switch (function)
        {
            case 2:
                if (y < 0.5)
                {
                    change = false;
                    target = -1.0;
                }
                break;
            case 3:
                if (y < 0.5 * x)
                {
                    change = false;
                    target = -1.0;
                }
                break;
            case 4:
                if (y < -0.5 * x + 0.5)
                {
                    change = false;
                    target = -1.0;
                }
                break;
        }

        if (change)
        {
            target = 1.0;
        }

        if (file.is_open())
        {
            file << x << "," << y << "," << target << endl;
        }

        targets.push_back(target);
    }

    p.trainPerceptron(epochs, inputs, targets, verbose);

    ofstream weigthsOutputs((filename + ".weights").c_str());

    if (!weigthsOutputs.is_open())
    {
        cerr << "ERROR: could't open the file \"" << (filename + ".weights") << "\"." << endl;
    }
    else
    {
        weigthsOutputs << "Accuracy: " << p.getBestAcc() * 100.0 << "%" << endl;
        weigthsOutputs << "Error: " << (1.0 - p.getBestAcc()) * 100.0 << "%" << endl;
        weigthsOutputs << endl;
        
        vector<double> weights = p.getWeights();

        for (size_t i = 0; i < weights.size(); i++)
        {
            weigthsOutputs << weights[i];

            if (i + 1 != weights.size())
            {
                weigthsOutputs << ",";
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 5)
    {
        cerr << "ERROR:" << endl;
        cerr << "  Usage: ./main <FUNCTION = 1 (AND function), 2 (y = 0.5), 3 (y = 0.5 * x), 4 (y = -0.5 * x + 0.5)> [if FUNCTION != 1 -> quantityOfPoints epochs <VERBOSE 0 (no verbose) 1 (verbose)>]" << endl;

        return 0;
    }

    int function = atoi(argv[1]);
    int points = 40;
    int epochs = 200;
    bool verbose = false;

    if (argc == 5)
    {
        points = atoi(argv[2]);
        epochs = atoi(argv[3]);

        if (atoi(argv[4]) == 1)
        {
            verbose = true;
        }
    }

    if (points < 0)
    {
        points = 40;
    }
    if (epochs < 0)
    {
        epochs = 200;
    }

    switch (function)
    {
        case 1:
            ANDFunction();
            break;
        case 2:
            functions(2, points, epochs, verbose);
            break;
        case 3:
            functions(3, points, epochs, verbose);
            break;
        case 4:
            functions(4, points, epochs, verbose);
            break;
        default:
            cerr << "ERROR: unknown function." << endl;
            cerr << "  Usage: ./main <FUNCTION = 1 (AND function), 2 (y = 0.5), 3 (y = 0.5 * x), 4 (y = -0.5 * x + 0.5)> [if FUNCTION != 1 -> quantityOfPoints epochs <VERBOSE 0 (no verbose) 1 (verbose)>]" << endl;
    }

    return 0;
}