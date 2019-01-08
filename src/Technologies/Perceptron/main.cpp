
#include <iostream>
#include <vector>
#include <fstream>

#include "Perceptron.h"

using namespace std;

void ANDFunction()
{
    string filename = "records/ANDFunction";
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

        weigthsOutputs.close();
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

    string filenameN1 = "records/function" + to_string(function) + "_-1.csv";
    string filenameP1 = "records/function" + to_string(function) + "_+1.csv";
    string filename = "records/function" + to_string(function) + ".csv";
    Perceptron p(2);
    vector<vector<double>> inputs;
    vector<double> targets;
    ofstream file(filename.c_str());
    ofstream fileN1(filenameN1.c_str());
    ofstream fileP1(filenameP1.c_str());

    if (!file.is_open() || !fileN1.is_open() || !fileP1.is_open())
    {
        cerr << "ERROR: could't open the file \"" << filename << "\" or \"" << filenameN1 << "\" or \"" << filenameP1 << "\"." << endl;
    }
    else
    {
        file << "x,y,target" << endl;
        fileN1 << "x,y" << endl;
        fileP1 << "x,y" << endl;
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
            case 5:
                if (y < 1 / (10 * x))
                {
                    change = false;
                    target = -1.0;
                }
                break;
            default:
                cerr << "ERROR: Unknown function." << endl;
        }

        if (change)
        {
            target = 1.0;
        }

        if (target < 0.0)
        {
            fileN1 << x << "," << y << endl;
        }
        else
        {
            fileP1 << x << "," << y << endl;
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

        weigthsOutputs.close();
    }

    if (file.is_open())
    {
        file.close();
    }
    if (fileN1.is_open())
    {
        fileN1.close();
    }
    if (fileP1.is_open())
    {
        fileP1.close();
    }
}

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 5)
    {
        cerr << "ERROR:" << endl;
        cerr << "  Usage: ./main <FUNCTION = 1 (AND function), 2 (y = 0.5), 3 (y = 0.5 * x), 4 (y = -0.5 * x + 0.5), 5 (y = 1 / (10 * x))> [if FUNCTION != 1 -> quantityOfPoints epochs <VERBOSE 0 (no verbose) 1 (verbose)>]" << endl;

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

    if (function == 1)
    {
        ANDFunction();
    }
    else if (function > 1 && function <= 5)
    {
        functions(function, points, epochs, verbose);
    }
    else
    {
        cerr << "ERROR: unknown function." << endl;
        cerr << "  Usage: ./main <FUNCTION = 1 (AND function), 2 (y = 0.5), 3 (y = 0.5 * x), 4 (y = -0.5 * x + 0.5), 5 (y = 1 / (10 * x))> [if FUNCTION != 1 -> quantityOfPoints epochs <VERBOSE 0 (no verbose) 1 (verbose)>]" << endl;
    }

    return 0;
}