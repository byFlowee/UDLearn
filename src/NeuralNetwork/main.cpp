
#include <iostream>

#include "neuralNetwork.h"

using namespace std;

void ANDFunction(bool showResults)
{
    cout << "AND function" << endl;
    cout << "------------" << endl;

    vector<int> nodes;
    
    nodes.push_back(2);
    nodes.push_back(10);
    nodes.push_back(1);
    
    NeuralNetwork nn(nodes);
    
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

    nn.train(inputs, expectedOutputs, 10000);
    
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

    vector<int> nodes;
    
    nodes.push_back(2);
    nodes.push_back(10);
    nodes.push_back(1);
    
    NeuralNetwork nn(nodes);
    
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

int main(int argc, char **argv)
{
    ANDFunction(true);
    XORFunction(true);

    return 0;
}