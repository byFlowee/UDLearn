
#include <iostream>

#include "neuralNetwork.h"

using namespace std;

void ANDFunction(bool showResults)
{
    cout << "AND function" << endl;
    cout << "------------" << endl;

    vector<int> nodes = {2, 10, 1};
    vector<double> dropout = {0.0, 0.3, 0.0};

    NeuralNetwork nn(nodes, dropout);
    nn.setLearningRate(0.1);

    Mat inputs1(1, 2);
    Mat inputs2(1, 2);
    Mat inputs3(1, 2);
    Mat inputs4(1, 2);
    Mat expectedOutputs1(1, 1);
    Mat expectedOutputs2(1, 1);
    vector<Mat> inputs;
    vector<Mat> expectedOutputs;

    /*
        * And function:
        *
        * Inputs: 0, 0
        * Expected output: 0
        *
        * Inputs: 0, 1
        * Expected output: 1
        *
        * Inputs: 1, 0
        * Expected output: 1
        *
        * Inputs: 1, 1
        * Expected output: 1
        */

    inputs1.set(0, 0, 0.0);
    inputs1.set(0, 1, 0.0);
    inputs2.set(0, 0, 0.0);
    inputs2.set(0, 1, 1.0);
    inputs3.set(0, 0, 1.0);
    inputs3.set(0, 1, 0.0);
    inputs4.set(0, 0, 1.0);
    inputs4.set(0, 1, 1.0);
    expectedOutputs1.set(0, 0, 0.0);
    expectedOutputs2.set(0, 0, 1.0);

    inputs.push_back(inputs1);
    inputs.push_back(inputs2);
    inputs.push_back(inputs3);
    inputs.push_back(inputs4);
    expectedOutputs.push_back(expectedOutputs1);
    expectedOutputs.push_back(expectedOutputs2);
    expectedOutputs.push_back(expectedOutputs2);
    expectedOutputs.push_back(expectedOutputs2);

    nn.train(inputs, expectedOutputs, 100000);

    Mat res1 = nn.forwardPropagation(inputs1);
    Mat res2 = nn.forwardPropagation(inputs2);
    Mat res3 = nn.forwardPropagation(inputs3);
    Mat res4 = nn.forwardPropagation(inputs4);

    if (showResults)
    {
        res1.print();
        cout << endl;
        res2.print();
        cout << endl;
        res3.print();
        cout << endl;
        res4.print();
        cout << endl;
    }

    if (res1.get(0, 0) < 0.5 && res2.get(0, 0) > 0.5 && res3.get(0, 0) > 0.5 && res4.get(0, 0) > 0.5)
    {
        cout << "Result correct!" << endl;
    }
    else
    {
        cout << "Result incorrect!" << endl;
    }
}

void XORFunction(bool showResults)
{
    cout << "XOR function" << endl;
    cout << "------------" << endl;

    vector<int> nodes = {2, 10, 1};
    vector<double> dropout = {0.0, 0.2, 0.0};

    NeuralNetwork nn(nodes, dropout);
    nn.setLearningRate(0.4);

    Mat inputs1(1, 2);
    Mat inputs2(1, 2);
    Mat inputs3(1, 2);
    Mat inputs4(1, 2);
    Mat expectedOutputs1(1, 1);
    Mat expectedOutputs2(1, 1);
    vector<Mat> inputs;
    vector<Mat> expectedOutputs;

    /*
        * Xor function:
        *
        * Inputs: 0, 0
        * Expected output: 0
        *
        * Inputs: 0, 1
        * Expected output: 1
        *
        * Inputs: 1, 0
        * Expected output: 1
        *
        * Inputs: 1, 1
        * Expected output: 0
        */

    inputs1.set(0, 0, 0.0);
    inputs1.set(0, 1, 0.0);
    inputs2.set(0, 0, 0.0);
    inputs2.set(0, 1, 1.0);
    inputs3.set(0, 0, 1.0);
    inputs3.set(0, 1, 0.0);
    inputs4.set(0, 0, 1.0);
    inputs4.set(0, 1, 1.0);
    expectedOutputs1.set(0, 0, 0.0);
    expectedOutputs2.set(0, 0, 1.0);

    inputs.push_back(inputs1);
    inputs.push_back(inputs2);
    inputs.push_back(inputs3);
    inputs.push_back(inputs4);
    expectedOutputs.push_back(expectedOutputs1);
    expectedOutputs.push_back(expectedOutputs2);
    expectedOutputs.push_back(expectedOutputs2);
    expectedOutputs.push_back(expectedOutputs1);

    nn.train(inputs, expectedOutputs, 100000);

    Mat res1 = nn.forwardPropagation(inputs1);
    Mat res2 = nn.forwardPropagation(inputs2);
    Mat res3 = nn.forwardPropagation(inputs3);
    Mat res4 = nn.forwardPropagation(inputs4);

    if (showResults)
    {
        res1.print();
        cout << endl;
        res2.print();
        cout << endl;
        res3.print();
        cout << endl;
        res4.print();
        cout << endl;
    }

    if (res1.get(0, 0) < 0.5 && res2.get(0, 0) > 0.5 && res3.get(0, 0) > 0.5 && res4.get(0, 0) < 0.5)
    {
        cout << "Result correct!" << endl;
    }
    else
    {
        cout << "Result incorrect!" << endl;
    }
}

void breakoutTest() {
    ifstream file;

    file.open("../breakout/breakout.csv");

    vector<string> line = NeuralNetwork::getNextLineAndSplitIntoTokens(file);

    for (size_t i = 0; i < line.size(); i++)
    {
        cout << line[i] << " ";
    }

    cout << endl;

    vector<int> nodes = {4, 20, 10, 1};
    vector<double> dropout = {0.0, 0.0, 0.0, 0.0};

    NeuralNetwork nn(nodes, dropout);
    nn.setLearningRate(0.1);

    vector<Mat> inputs;
    vector<Mat> expectedOutputs;

    for (int i = 0; i < 4990; i++)
    {
        line.clear();
        line = NeuralNetwork::getNextLineAndSplitIntoTokens(file);

        Mat inputs1(1, 4);
        Mat expectedOutputs1(1, 1);

        inputs1.set(0, 0, atoi(line[0].c_str()));
        inputs1.set(0, 1, atoi(line[1].c_str()));
        inputs1.set(0, 2, atoi(line[2].c_str()));
        inputs1.set(0, 3, atoi(line[3].c_str()));
        expectedOutputs1.set(0, 0, atoi(line[4].c_str()));

        inputs.push_back(inputs1);
        expectedOutputs.push_back(expectedOutputs1);
    }

    nn.train(inputs, expectedOutputs, 100);

    file.close();
}

void crossValidationTest() {
    vector<int> nodes = {4, 10, 1};
    vector<double> dropout = {0.0, 0.2, 0.0};

    NeuralNetwork nn(nodes, dropout);
    nn.setLearningRate(0.1);

    nn.crossFoldValidation(30, 10, "../breakout/breakout.csv");
}

void dropoutTest() {
    vector<int> nodes = {2, 2, 1};
    vector<double> dropout = {0.0, 0.4, 0.0};

    NeuralNetwork dropoutNet(nodes, dropout);
    NeuralNetwork regularNet(nodes);

    ifstream file1, file2;
    file1.open("../../Documentation/CSVFiles/function5_+1.csv");
    file2.open("../../Documentation/CSVFiles/function5_-1.csv");

    vector<string> line;
    
    NeuralNetwork::getNextLineAndSplitIntoTokens(file1);
    NeuralNetwork::getNextLineAndSplitIntoTokens(file2);

    vector<Mat> inputs;
    vector<Mat> expectedOutputs;

    for (int i = 0; i < 68; i++)
    {
        line.clear();
        line = NeuralNetwork::getNextLineAndSplitIntoTokens(file1);

        Mat inputs1(1, 2);
        Mat expectedOutputs1(1, 1);

        inputs1.set(0, 0, atoi(line[0].c_str()));
        inputs1.set(0, 1, atoi(line[1].c_str()));
        expectedOutputs1.set(0, 0, 1);
        
        inputs.push_back(inputs1);
        expectedOutputs.push_back(expectedOutputs1);
        
        line.clear();
        line = NeuralNetwork::getNextLineAndSplitIntoTokens(file2);

        Mat inputs2(1, 2);
        Mat expectedOutputs2(1, 1);

        inputs1.set(0, 0, atoi(line[0].c_str()));
        inputs1.set(0, 1, atoi(line[1].c_str()));
        expectedOutputs1.set(0, 0, 1);

        inputs.push_back(inputs2);
        expectedOutputs.push_back(expectedOutputs2);
    }

    cout << fixed;
    cout.precision(15);
    regularNet.train(inputs, expectedOutputs, 1000);
    dropoutNet.train(inputs, expectedOutputs, 1000);
}

int main(int argc, char **argv)
{

    if (argc == 2){ 

        switch (atoi(argv[1])) {
            case 1:
                ANDFunction(true);
                break;
            case 2:
                XORFunction(true);
                break;
            case 3:
                breakoutTest();
                break;
            case 4:
                crossValidationTest();
                break;
            case 5:
                dropoutTest();
                break;
            default:
                cout << "ExperimentID debe ser un valor entre 1 y 5" << endl;
        }
    }
    else {
        cout << "Usage: ./main <ExperimentID>" << endl;  
    }

    return 0;
}