
#include <iostream>

#include "GeneticNN.h"
#include "DNA.h"
#include "UtilG.h"
#include "../NeuralNetwork/neuralNetwork.h"

using namespace std;

void flattenMatrix()
{
    vector<Mat> a;

    for (int i = 0; i < 10; i++)
    {
        Mat m(i + 2, i + 1, i);

        m.set(0, 0, 99.0);
        m.set(i, 0, 22.0);
        m.print();
        cout << endl;

        a.push_back(m);
    }

    cout << endl;

    Mat flattened = UtilG::flattenMatices(a);

    cout << "Flattened matrix:" << endl;
    flattened.print();
    cout << endl;
}

void flattenManuallyNNWeightsAndBias()
{
    vector<int> topology = {2, 3, 4};

    NeuralNetwork nn(topology);

    vector<Mat> nnWeights = nn.getWeights();
    vector<Mat> nnBias = nn.getBias();

    cout << endl;
    cout << "Weights:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnWeights.size(); i++)
    {
        nnWeights[i].print();
        cout << endl << "-----------------------------" << endl;
    }

    cout << endl;
    cout << "Bias:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnBias.size(); i++)
    {
        nnBias[i].print();
        cout << endl << "-----------------------------" << endl;
    }

    Mat flattenedWeights = UtilG::flattenMatices(nnWeights);
    Mat flattenedBias = UtilG::flattenMatices(nnBias);

    cout << endl;
    cout << "Flattened weights: " << endl;
    flattenedWeights.print();

    cout << endl << endl;
    cout << "Flattened bias: " << endl;
    flattenedBias.print();

    Mat unionWeightsBias = UtilG::unionOfFlattenedMatrices(flattenedWeights, flattenedBias);

    cout << endl << endl;
    cout << "Union between flattened weights and flattened bias: " << endl;
    unionWeightsBias.print();

    vector<vector<Mat>> unflattenedWeightsAndBias = UtilG::unflattenMatrices(topology, unionWeightsBias);

    cout << endl << endl;
    cout << "Weights after unflatten:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < unflattenedWeightsAndBias[0].size(); i++)
    {
        unflattenedWeightsAndBias[0][i].print();
        cout << endl << "-----------------------------" << endl;
    }

    cout << endl;
    cout << "Bias after unflatten:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < unflattenedWeightsAndBias[1].size(); i++)
    {
        unflattenedWeightsAndBias[1][i].print();
        cout << endl << "-----------------------------" << endl;
    }
}

void flattenUtilGNNWeightsAndBias()
{
    vector<int> topology = {2, 3, 4};

    NeuralNetwork nn(topology);
    
    vector<Mat> nnWeights = nn.getWeights();
    vector<Mat> nnBias = nn.getBias();

    cout << endl;
    cout << "Weights:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnWeights.size(); i++)
    {
        nnWeights[i].print();
        cout << endl << "-----------------------------" << endl;
    }

    cout << endl;
    cout << "Bias:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnBias.size(); i++)
    {
        nnBias[i].print();
        cout << endl << "-----------------------------" << endl;
    }

    Mat representativeVector = UtilG::getARepresentativeVectorOfNeuralNetwork(nn);

    cout << endl << endl;

    representativeVector.set(0, 0, 0.1234);

    UtilG::setRepresentativeVectorOnNeuralNetwork(representativeVector, nn);

    nnWeights = nn.getWeights();
    nnBias = nn.getBias();

    cout << "Weights:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnWeights.size(); i++)
    {
        nnWeights[i].print();
        cout << endl << "-----------------------------" << endl;
    }

    cout << endl;
    cout << "Bias:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnBias.size(); i++)
    {
        nnBias[i].print();
        cout << endl << "-----------------------------" << endl;
    }
}

void DNACrossoverAndMutation()
{
    Mat mA(1, 5);
    Mat mB = UtilG::getRandomMatrix(1, 5);

    DNA a(mA);
    DNA b(mB);

    cout << "DNA a:" << endl;
    a.getGenes().print();

    cout << endl;

    cout << "DNA b:" << endl;
    b.getGenes().print();

    cout << endl;

    Mat crossover1 = a.crossover(b).getGenes();
    Mat crossover2 = b.crossover(a).getGenes();
    Mat crossover3 = b.crossover(b).getGenes();

    cout << "Crossover a->b:" << endl;
    crossover1.print();
    cout << endl;
    cout << "Crossover b->a:" << endl;
    crossover2.print();
    cout << endl;
    cout << "Crossover b->b:" << endl;
    crossover3.print();
    cout << endl;

    cout << endl << "Mutation:" << endl;
    srand(time(NULL));

    for (int i = 0; i < 100; i++)
    {
        DNA mutated = a;

        mutated.mutate();

        if (a == mutated)
        {
            cout << "Same";
        }
        else
        {
            cout << endl;
            cout << "Different!" << endl;
            cout << "a: ";
            a.getGenes().print();
            cout << endl << "muteted: ";
            mutated.getGenes().print();
            cout << endl << endl;
        }
    }

    cout << endl;
}

int main ()
{
    DNACrossoverAndMutation();

    return 0;
}