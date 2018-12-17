
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

NeuralNetwork play(const vector<int> &topology, Game game, int maxGenerations, int population, double mutation)
{
    int score = 0;
    int steps = 0;
    GeneticNN geneticAlg(topology, game, population);
    NeuralNetwork nn(topology);
    vector<vector<Mat>> weightsAndBias;
    DNA best;
    int bestScore = -1;
    int fitnessValue = 0;
    bool improvedScore = true;
    bool improvedFitness = true;

    geneticAlg.createPopulation();
    geneticAlg.setMutation(mutation);

    for (int i = 0; i < maxGenerations; i++)
	{
        geneticAlg.computeFitness();

        int fitnessAux = geneticAlg.getCurrentBestDNAFitness();

        if (fitnessAux > fitnessValue)
        {
            improvedFitness = true;
        }
        else
        {
            improvedFitness = false;
        }
			
        fitnessValue = fitnessAux;
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

        if (playerResults[0] > score)
        {
            improvedScore = true;
        }
        else
        {
            improvedScore = false;
        }

        score = playerResults[0];
        steps = playerResults[1];

        //cout << "Generation " << geneticAlg.getCurrentGeneration() << ": " << best << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "------------------- Generation " << geneticAlg.getCurrentGeneration() + 1 << endl;

        if (improvedFitness)
        {
            cout << "------------------- Best fitness = \033[1;32m" << fitnessValue << "\033[0m" << endl;
        }
        else
        {
            cout << "------------------- Best fitness = \033[1;31m" << fitnessValue << "\033[0m" << endl;
        }

        if (improvedScore)
        {
            cout << "------------------- Score playing = \033[1;32m" << score << "\033[0m" << endl;
        }
        else
        {
            cout << "------------------- Score playing = \033[1;31m" << score << "\033[0m" << endl;
        }

        cout << "------------------- Steps playing = " << steps << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << endl;

        if (bestScore < score)
        {
            bestScore = score;
            best = currentBest;
        }

        if (game == Game::breakout && score == 864)
        {
            cout << "MAX SCORE IN BREAKOUT: 864!" << endl;
            break;
        }
        
        geneticAlg.nextGeneration();
    }

    Mat flattenedWeightsAndBiasBest = best.getGenes();

    cout << "Weights and Bias flattened of the best DNA found (score = " << bestScore << "): ";
    flattenedWeightsAndBiasBest.print(", ");
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
    if (argc != 4 && argc != 5)
    {
        cerr << "ERROR: Usage: ./main <GAME = 1 (breakout), 2 (boxing), 3 (demon attack), 4 (star gunner)> maxGenerations population [mutation]" << endl;

        return 0;
    }

    // representation SCORE GAME

    // 3190 Demon Attack
    //
    //vector<double> representation3190DemonAttack = {0.813133, -0.774577, -0.0958515, -0.177008, -0.148763, 0.385405, -0.397537, -0.685122, 0.273571, 0.077728, -0.970402, 0.558187, 0.987247, 0.0811712, 0.859851, 0.551545, 0.138882, 0.233675, 0.785428, 0.786788, 0.83853, 0.682145, -0.461455, 0.225344, 0.707928, 0.080523, 0.772754, -0.0670632, -0.389047, -0.4632, 0.797971, -0.706557, -0.0331996, 0.731363, 0.583559, -0.00152647, 0.578275, -0.671047, 0.177851, -0.659175, -0.50879, 0.140976, -0.935709, -0.977578, 0.765607, 0.968304, 0.740836, 0.754916, 0.141921, 0.662001, 0.821084, 0.108215, -0.308102, 0.786728, 0.581076, 0.799861, 0.457467, -0.61663, 0.00669438, 0.150084, 0.30884, 0.635624, -0.988787, 0.682393, 0.506785, -0.544535, -0.906322, 0.903477, 0.0197724, 0.994218, -0.644961, 0.338245, -0.521022, -0.276407, 0.118459, 0.938438, 0.188411, 0.787487, 0.833682, 0.893877, -0.136939, 0.945431, -0.146257, -0.206365, -0.985604, 0.999156, 0.342119, -0.40059, 0.570956, 0.929156, -0.422304, 0.302697, 0.589026, -0.0159715, 0.155438, -0.671096, -0.816319, 0.665957, 0.958926, -0.776088, 0.578297, 0.378642, -0.763524, -0.204632, 0.036328, -0.371469, -0.413487, 0.0527279, 0.478231, -0.206874, 0.990611, 0.865507, -0.925038, 0.470283, 0.024437, -0.529285, -0.143291, 0.252637, 0.910893, 0.851966, 0.733639, -0.284878, 0.398573, 0.966958, -0.753468, -0.860247, -0.744669, 0.00956956, 0.712016, -0.317589, 0.716868, 0.465884, -0.471777, 0.777568, 0.0986195, -0.31952, 0.177318, -0.586038, 0.827953, -0.177882, -0.761006, -0.320187, 0.730627, 0.945645, -0.337081, 0.346766, -0.596686, 0.619535, -0.214158, 0.486485, 0.947294, 0.994306, 0.140864, -0.373836, -0.818308, 0.900758, 0.717657, -0.411066, 0.948215, -0.428374, -0.348721, 0.763642, 0.257343, 0.590993, -0.15654, -0.701238, -0.55898, -0.414373, -0.286581, 0.577236, 0.557889, 0.919812, -0.912463, 0.276931, 0.230138, -0.16541, -0.670619, -0.718183, -0.729915, -0.95846, 0.152924, 0.925626, 0.915449, 0.969908, 0.472136, 0.576542, 0.681185, 0.317707, -0.465837, -0.0899306, -0.380534, 0.799333, 0.329391, 0.926844, -0.635055, 0.491557, 0.176421, 0.661799, 0.37357, -0.0616692, 0.446104, 0.484929, 0.141433, 0.351259, 0.56951, 0.0343798, 0.912932, 0.0224349, 0.778304, 0.639689, 0.0141142, -0.226282, -0.599603, 0.879284, 0.467018, -0.882895, 0.0267043, -0.385707, 0.0623254, -0.249108, -0.595004, 0.569403, -0.25369, -0.163609, 0.963592, 0.940551, -0.32936, 0.476597, 0.917219, 0.622242, 0.0298533, 0.26883, -0.739248, 0.229679, 0.591509, 0.470362, 0.980728, 0.839376, 0.729154, 0.00208779, -0.749419, 0.693697, 0.398982, -0.989627, 0.672831, 0.713558, -0.47649, -0.936477, -0.178523, 0.56229, -0.739597, -0.341913, 0.267627, 0.36464, 0.34256, -0.862573, -0.882032, 0.822427, -0.628767, -0.092459, 0.699951, -0.971049, 0.0273444, -0.882376, 0.273192, 0.796176, 0.790965, 0.298693, 0.249541, 0.788562, 0.0129524, 0.0466026, 0.778239, -0.923414, -0.8489, 0.478173, 0.618489, 0.203344, -0.364246, 0.38548, -0.739554, 0.40849, 0.917045, 0.63318, 0.5459, 0.602741, -0.240708, 0.0259421, 0.0221984, 0.38463, -0.943975, 0.275937, -0.468389, -0.890145, -0.317147, -0.284398, -0.734654, 0.840026, 0.0587229, -0.492424, 0.993937, 0.218752, 0.0150084, 0.356243, 0.774888, -0.388846, 0.324565, -0.0123203, 0.840718, -0.623888, 0.833343, -0.45765, 0.725452, 0.131956, 0.126928, 0.565363, 0.68981, 0.823393, 0.64916, -0.307702, 0.975952, 0.4124, -0.746872, 0.447231, 0.916753, -0.325485, 0.325484, 0.428285, 0.409799, 0.998879, -0.431213, 0.349894, 0.370854, 0.490039, 0.661061, -0.819021, 0.670001, 0.344118, -0.84587, 0.708872, -0.316102, 0.840828, -0.308214, 0.879197, 0.303116, 0.129515, -0.962328, 0.106951, -0.290449, 0.694407, 0.527012, 0.891579, 0.619897, 0.295789, 0.848505, -0.803352, -0.283879, 0.658266, 0.234472, -0.717275, 0.0489245, 0.560794, 0.386285, 0.359433, 0.387056, 0.701188, -0.204458, 0.152026, 0.0453204, -0.290929, -0.272418, -0.479164, -0.0628553, 0.780606, -0.861432, 0.586488, 0.740389, 0.37049, 0.770554, -0.336533, 0.358162, -0.76342, 0.725457, 0.570405, 0.727585, 0.353515, 0.43566};
    //Mat representation3190DemonAttackMat = UtilG::getMatFromVector(representation3190DemonAttack);
    //vector<int> topology3190DemonAttack = {128, 3};
    //NeuralNetwork nn3190DemonAttack(topology3190DemonAttack);
    //UtilG::setRepresentativeVectorOnNeuralNetwork(representation3190DemonAttackMat, nn3190DemonAttack);
    //Player::playDemonAttack(nn3190DemonAttack, true);

    // 864 Breakout
    //
    //vector<double> representation864Breakout = {0.910057, 0.967478, -0.356777, 0.0107037, -0.916711, 0.0107037, -0.9124, 0.95785, 0.696089, 0.941645};
    //Mat representation864BreakoutMat = UtilG::getMatFromVector(representation864Breakout);
    //vector<int> topology864Breakout = {4, 2};
    //NeuralNetwork nn864Breakout(topology864Breakout);
    //UtilG::setRepresentativeVectorOnNeuralNetwork(representation864BreakoutMat, nn864Breakout);
    //Player::playBreakout(nn864Breakout, true);

    vector<int> topologyBreakout = {4, 2};
    vector<int> topologyBoxing = {};
    vector<int> topologyDemonAttack = {128, 3};
    vector<int> topologyStarGunner = {};

    //setbuf(stdout, NULL);

    // Redirect error output to /dev/null -> all messages in ALE are displayed in the error output.......
    int save_out = dup(STDERR_FILENO);
    int devNull = open("/dev/null", O_WRONLY);
    bool error = false;
    dup2(devNull, STDERR_FILENO);
    double mutation = 0.05;

    if (argc == 5)
    {
        mutation = atof(argv[4]);
    }

    Game game = (Game)atoi(argv[1]);

    switch (game)
    {
        case Game::breakout:
            play(topologyBreakout, game, atoi(argv[2]), atoi(argv[3]), mutation);
            break;
        case Game::boxing:
            play(topologyBoxing, game, atoi(argv[2]), atoi(argv[3]), mutation);
            break;
        case Game::demonAttack:
            play(topologyDemonAttack, game, atoi(argv[2]), atoi(argv[3]), mutation);
            break;
        case Game::starGunner:
            play(topologyStarGunner, game, atoi(argv[2]), atoi(argv[3]), mutation);
            break;
        default:
            error = true;
    }

    // Restore error output
    dup2(save_out, STDERR_FILENO);

    if (error)
    {
        cerr << "ERROR: unknown game." << endl;
    }

    return 0;
}