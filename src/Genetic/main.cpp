
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "GeneticNN.h"
#include "DNA.h"
#include "UtilG.h"
#include "../NeuralNetwork/neuralNetwork.h"
#include "../DataLoader/DataLoader.h"
#include "Player.h"

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

NeuralNetwork play(const vector<int> &topology, Game game, int maxGenerations, int population)
{
    int score = 0;
    int steps = 0;
    GeneticNN geneticAlg(topology, game, maxGenerations, population);
    NeuralNetwork nn(topology);
    vector<vector<Mat>> weightsAndBias;
    DNA best;
    int bestScore = -1;

    geneticAlg.createPopulation();
    geneticAlg.setMutation(0.05);

    for (int i = 0; i < maxGenerations; i++)
	{
        geneticAlg.computeFitness();
			
        int fitnessValue = geneticAlg.getCurrentBestDNAFitness();
        DNA currentBest = geneticAlg.getCurrentBestDNA();
        Mat genes = currentBest.getGenes();
        vector<int> playerResults;

        // Weights and bias of the best Neural Network
        weightsAndBias = UtilG::setRepresentativeVectorOnNeuralNetwork(genes, nn);

        //score = Player::playBreakout(nn);
        //playerResults = Player::playBreakout(nn, true);   // If you want to see the best DNA of the generation. Can't be closed and continue.. It has to end.

        switch (game)
        {
            case Game::breakout:
                playerResults = Player::playBreakout(nn);           // If you don't want to see the best DNA.
                break;
            case Game::boxing:
                playerResults = Player::playBoxing(nn);           // If you don't want to see the best DNA.
                break;
            case Game::demonAttack:
                playerResults = Player::playDemonAttack(nn);           // If you don't want to see the best DNA.
                break;
            case Game::starGunner:
                playerResults = Player::playStarGunner(nn);           // If you don't want to see the best DNA.
                break;
            default:
                cerr << "ERROR: game unknown." << endl;
        }

        score = playerResults[0];
        steps = playerResults[1];

        //cout << "Generation " << geneticAlg.getCurrentGeneration() << ": " << best << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "------------------- Generation " << geneticAlg.getCurrentGeneration() + 1 << endl;
        cout << "------------------- Best fitness = " << fitnessValue << endl;
        cout << "------------------- Score playing = " << score << endl;
        cout << "------------------- Steps playing = " << steps << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << endl;

        if (bestScore < score)
        {
            bestScore = score;
            best = currentBest;
        }

        /*
        if (score == 864)
        {
            cout << "MAX SCORE! 864!" << endl;
            break;
        }
        */
        
        geneticAlg.nextGeneration();
    }

    //Mat flattenedWeightsAndBiasBest = geneticAlg.getCurrentBestDNA().getGenes();
    Mat flattenedWeightsAndBiasBest = best.getGenes();

    cout << "Weights and Bias flattened of the best DNA found (score = " << bestScore << "): ";
    flattenedWeightsAndBiasBest.print();
    cout << endl << endl;

    UtilG::setRepresentativeVectorOnNeuralNetwork(flattenedWeightsAndBiasBest, nn);

    // See best DNA of last generation
    cout << "Best neural network is playing!" << endl;
    switch (game)
    {
        case Game::breakout:
            Player::playBreakout(nn, true);
            break;
        case Game::boxing:
            Player::playBoxing(nn, true);
            break;
        case Game::demonAttack:
            Player::playDemonAttack(nn, true);
            break;
        case Game::starGunner:
            Player::playStarGunner(nn, true);
            break;
        default:
            cerr << "ERROR: game unknown." << endl;
    }

    return nn;
}

int main (int argc, char **argv)
{
    if (argc != 4)
    {
        cerr << "ERROR: Usage: ./main <GAME = 1 (breakout), 2 (), 3 (demon attack), 4 ()> maxGenerations population" << endl;

        return 0;
    }

    vector<int> topologyBreakout = {4, 1};
    vector<int> topologyBoxing = {};
    vector<int> topologyDemonAttack = {128, 32, 3};
    vector<int> topologyStarGunner = {};

    setbuf(stdout, NULL);

    // Redirect error output to /dev/null -> all messages in ALE are displayed in the error output.......
    int save_out = dup(STDERR_FILENO);
    int devNull = open("/dev/null", O_WRONLY);
    dup2(devNull, STDERR_FILENO);

    if (atoi(argv[1]) > 4 || atoi(argv[1]) < 1)
    {
        // Restore error output
        dup2(save_out, STDERR_FILENO);

        cerr << "ERROR: unknown game." << endl;

        return 0;
    }

    Game game = (Game)atoi(argv[1]);

    switch (game)
    {
        case Game::breakout:
            play(topologyBreakout, game, atoi(argv[2]), atoi(argv[3]));
            break;
        case Game::boxing:
            play(topologyBoxing, game, atoi(argv[2]), atoi(argv[3]));
            break;
        case Game::demonAttack:
            play(topologyDemonAttack, game, atoi(argv[2]), atoi(argv[3]));
            break;
        case Game::starGunner:
            play(topologyStarGunner, game, atoi(argv[2]), atoi(argv[3]));
            break;
        default:
            cerr << "ERROR: unknown game." << endl;
    }

    // Restore error output
    dup2(save_out, STDERR_FILENO);


    return 0;
}