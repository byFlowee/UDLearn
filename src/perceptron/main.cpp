
#include <iostream>
#include <vector>

#include "Perceptron.h"

using namespace std;

void ANDFunction()
{
    Perceptron p(2);

    vector<vector<double>> inputs;
    vector<double> targets = {-1.0, -1.0, -1.0, 1.0};

    inputs.push_back({0.0, 0.0});
    inputs.push_back({0.0, 1.0});
    inputs.push_back({1.0, 0.0});
    inputs.push_back({1.0, 1.0});

    p.trainPerceptron(200, inputs, targets);

    cout << endl;
    cout << "A B : A^B" << endl;
    cout << "---------" << endl;
    cout << "0 0 :  " << p.getPrediction({0.0, 0.0}) << endl;
    cout << "0 1 :  " << p.getPrediction({0.0, 1.0}) << endl;
    cout << "1 0 :  " << p.getPrediction({1.0, 0.0}) << endl;
    cout << "1 1 :  " << p.getPrediction({1.0, 1.0}) << endl;
}

double getRandomDouble(double min, double max)
{
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// Function: y = 0.5
void function1(int points, int epochs, bool verbose = false)
{
    Perceptron p(2);
    vector<vector<double>> inputs;
    vector<double> targets;

    for (int i = 0; i < points; i++)
    {
        double x = getRandomDouble(0.0, 1.0);
        double y = getRandomDouble(0.0, 1.0);

        inputs.push_back({x, y});

        if (y < 0.5)
        {
            targets.push_back(-1.0);
        }
        else
        {
            targets.push_back(1.0);
        }
    }

    p.trainPerceptron(epochs, inputs, targets, verbose);
}

// Function: y = 0.5 * x
void function2(int points, int epochs, bool verbose = false)
{
    Perceptron p(2);
    vector<vector<double>> inputs;
    vector<double> targets;

    for (int i = 0; i < points; i++)
    {
        double x = getRandomDouble(0.0, 1.0);
        double y = getRandomDouble(0.0, 1.0);

        inputs.push_back({x, y});

        if (y < 0.5 * x)
        {
            targets.push_back(-1.0);
        }
        else
        {
            targets.push_back(1.0);
        }
    }

    p.trainPerceptron(epochs, inputs, targets, verbose);
}

// Function: y = -0.5 * x + 0.5
void function3(int points, int epochs, bool verbose = false)
{
    Perceptron p(2);
    vector<vector<double>> inputs;
    vector<double> targets;

    for (int i = 0; i < points; i++)
    {
        double x = getRandomDouble(0.0, 1.0);
        double y = getRandomDouble(0.0, 1.0);

        inputs.push_back({x, y});

        if (y < -0.5 * x + 0.5)
        {
            targets.push_back(-1.0);
        }
        else
        {
            targets.push_back(1.0);
        }
    }

    p.trainPerceptron(epochs, inputs, targets, verbose);
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
            function1(points, epochs, verbose);
            break;
        case 3:
            function2(points, epochs, verbose);
            break;
        case 4:
            function3(points, epochs, verbose);
            break;
        default:
            cerr << "ERROR: unknown function." << endl;
            cerr << "  Usage: ./main <FUNCTION = 1 (AND function), 2 (y = 0.5), 3 (y = 0.5 * x), 4 (y = -0.5 * x + 0.5)> [if FUNCTION != 1 -> quantityOfPoints epochs <VERBOSE 0 (no verbose) 1 (verbose)>]" << endl;
    }

    return 0;
}