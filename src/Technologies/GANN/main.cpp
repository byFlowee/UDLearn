
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <limits>

#include "UtilG.h"
#include "../NeuralNetwork/neuralNetwork.h"
#include "Player.h"

#include "GANNWrapper.h"

using namespace std;

int main (int argc, char **argv)
{
    if (argc == 4)
    {
        Game game = (Game)atoi(argv[1]);
        bool visualization = atoi(argv[2]) == 1;
        int steps = atoi(argv[3]);

        if (game == Game::breakout)
        {
            vector<double> weightsAndBiases = {-0.0475447, -1.44612, -1.03555, 1.19032, -0.0180972, 1.39592, -1.65275, -0.400388, 0.313902, -0.911924};
            Mat weightsAndBiasesMat = UtilG::getMatFromVector(weightsAndBiases);
            vector<int> topology = {4, 2};
            NeuralNetwork nn(topology);
            UtilG::setRepresentativeVectorOnNeuralNetwork(weightsAndBiasesMat, nn);
            vector<int> values = Player::playBreakout(nn, visualization, steps);

            cout << "Score: " << values[0] << endl;
            cout << "Steps: " << values[1] << endl;
        }
        else if (game == Game::boxing)
        {
            //vector<double> weightsAndBiases = {1.84625, -0.261621, -0.752879, 0.933104, 0.998058, -1.75949, -1.09565, -1.5496, -0.0616241, -0.0961609, 0.410759, 1.81586, 0.861455, 0.358694, 1.99665, 1.26765, -0.510498, -1.94463, -0.207363, -1.83149, 0.809997, -1.56389, 1.82184, 0.0756448, -0.751285};
            vector<double> weightsAndBiases = {0.368054, 0.657731, 0.295209, -1.06667, 0.565548, 0.556453, 0.557153, 0.577881, -0.752881, -1.40455, 0.694681, -1.28186, -0.091806, -1.38144, -0.883285, -0.52821, -0.652945, 0.794675, 0.707091, -1.12105, -0.121103, -0.921668, -0.262731, -1.07216, 0.404015};
            Mat weightsAndBiasesMat = UtilG::getMatFromVector(weightsAndBiases);
            vector<int> topology = {4, 5};
            NeuralNetwork nn(topology);
            UtilG::setRepresentativeVectorOnNeuralNetwork(weightsAndBiasesMat, nn);
            vector<int> values = Player::playBoxing(nn, visualization, steps);

            cout << "Score: " << values[0] << endl;
            cout << "Steps: " << values[1] << endl;
        }
        else if (game == Game::demonAttack)
        {
            vector<double> weightsAndBiases = {-0.787538, -0.949064, 0.310924, -1.32299, 0.71908, -0.0709453, 1.49602, -0.094367, 0.500397, 0.404399, -0.263846, -0.431822};
            Mat weightsAndBiasesMat = UtilG::getMatFromVector(weightsAndBiases);
            vector<int> topology = {3, 3};
            NeuralNetwork nn(topology);
            UtilG::setRepresentativeVectorOnNeuralNetwork(weightsAndBiasesMat, nn);
            vector<int> values = Player::playDemonAttack(nn, visualization, steps);

            cout << "Score: " << values[0] << endl;
            cout << "Steps: " << values[1] << endl;
        }
        else if (game == Game::starGunner)
        {
            vector<double> weightsAndBiases = {0.171788, 0.954718, 0.434003, -0.57256, -0.806731, -0.435586, -0.0937895, 0.310619, -0.308204, -0.517226, -0.104929, -0.893615, -0.49165, -0.170597, -0.138965, 0.0929738, -1.02856, 0.633477, -1.24683, 0.756926, 0.57379, 0.981531, -0.586579, 0.191376, 0.652615, 0.0737398, 0.0531118, 0.774934, 0.277529, -1.083, -0.438313, -0.539808, 0.045639, -0.110905, 0.160107, 0.532854, -0.144477, -0.390065, 0.625603, -0.0595559, -0.444677, -0.655645, -0.0493453, 0.590344, 0.639938, -0.658841, 0.356061, -0.475919, 0.0739151, 0.755685, -0.537857, -0.483144, 0.0420367, 0.519662, 0.38397, -0.284232, -0.902964, 0.36212, 0.024936, -1.01229, 1.26559, -0.695163, 0.4479, -0.61269, 0.574092};
            Mat weightsAndBiasesMat = UtilG::getMatFromVector(weightsAndBiases);
            vector<int> topology = {12, 5};
            NeuralNetwork nn(topology);
            UtilG::setRepresentativeVectorOnNeuralNetwork(weightsAndBiasesMat, nn);
            vector<int> values = Player::playStarGunner(nn, visualization, steps);

            cout << "Score: " << values[0] << endl;
            cout << "Steps: " << values[1] << endl;
        }
        else
        {
            cerr << "ERROR: unknown game." << endl;
        }

        return 0;
    }
    else if (argc != 6)
    {
        cerr << "ERROR:" << endl;
        cerr << "  Usage_1 (train): ./main <GAME = 1 (breakout), 2 (boxing), 3 (demon attack), 4 (star gunner)> maxGenerations population filenameToSaveRecords gameSteps" << endl;
        cerr << "  Usage_2 (play):  ./main <GAME = 1 (breakout), 2 (boxing), 3 (demon attack), 4 (star gunner)> <VISUALIZATION 0 (disabled) 1 (enabled)> gameSteps" << endl;

        return 0;
    }

    // Redirect error output to /dev/null -> all messages in ALE are displayed in the error output.......
    int save_out = dup(STDERR_FILENO);
    int devNull = open("/dev/null", O_WRONLY);
    bool error = false;

    dup2(devNull, STDERR_FILENO);

    int generations = atoi(argv[2]);
    int population = atoi(argv[3]);
    string filenameToSaveRecords(argv[4]);
    vector<double> initialElitism;
    vector<WeightInitializationRange> weightsInitialization;

    Game game = (Game)atoi(argv[1]);
    int steps = atoi(argv[5]);

    if (game == Game::breakout)
    {
        //initialElitism = {0.189794, -0.560286, -0.460457, 0.823264, -0.65252, 0.115743, -0.194485, 0.109773, 0.382016, 0.0633527};

        //weightsInitialization.resize(10);

        //weightsInitialization[0] = WeightInitializationRange(-0.04754, 0.47709);
        //weightsInitialization[1] = WeightInitializationRange(-1.44612, -0.54442);
        //weightsInitialization[2] = WeightInitializationRange(-1.03555, 0.51202);
        //weightsInitialization[3] = WeightInitializationRange(-0.50003, 1.19032);
        //weightsInitialization[4] = WeightInitializationRange(-0.65252, -0.0181);
        //weightsInitialization[5] = WeightInitializationRange(0.12, 1.39592);
        //weightsInitialization[6] = WeightInitializationRange(-1.65275, 0.26552);
        //weightsInitialization[7] = WeightInitializationRange(-0.40039, 0.83252);
        //weightsInitialization[8] = WeightInitializationRange(0.11188, 0.38202);
        //weightsInitialization[9] = WeightInitializationRange(-0.91, 0.19889);

        double mutationRate = 0.2;
        double crossoverRate = 0.7;
        size_t elitism = (population < 100 ? 1 : population / 100);
        size_t weightsFactor = 1;
        vector<int> topologyBreakout = {4, 2};

        GANNWrapper gannWrapper(topologyBreakout, Game::breakout, steps, generations, population, elitism, weightsFactor, filenameToSaveRecords);

        //gannWrapper.getGANN().setInitialElitism(initialElitism);
        //gannWrapper.getGANN().setWeightInitializationRange(weightsInitialization);
        gannWrapper.getGANN().setMutationRate(mutationRate);
        gannWrapper.getGANN().setCrossoverRate(crossoverRate);
        
        gannWrapper.writeInScore("Configuration\n");
        gannWrapper.writeInScore("---------------------------\n");
        gannWrapper.writeInScore("Generations: " + to_string(generations) + "\n");
        gannWrapper.writeInScore("Population: " + to_string(population) + "\n");
        gannWrapper.writeInScore("Mutation rate: " + to_string(mutationRate) + "\n");
        gannWrapper.writeInScore("Elitism: " + to_string(elitism) + "\n");
        gannWrapper.writeInScore("Weights Factor: " + to_string(weightsFactor) + "\n");
        gannWrapper.writeInScore("Game: Breakout\n");
        gannWrapper.writeInScore("Topology: ");
        UtilG::printVector(topologyBreakout, gannWrapper.getBestFitnessAndScoreFileTxt());
        gannWrapper.writeInScore("\n\n");

        NeuralNetwork nn = gannWrapper.trainAndGetBestNN();

        cout << "Best neural network is playing!" << endl;
        Player::playBreakout(nn, true, steps);
    }
    else if (game == Game::boxing)
    {
        double mutationRate = 0.2;
        double crossoverRate = 0.7;
        size_t elitism = 5;
        size_t weightsFactor = 2;
        vector<int> topologyBoxing = {4, 5};

        GANNWrapper gannWrapper(topologyBoxing, Game::boxing, steps, generations, population, elitism, weightsFactor, filenameToSaveRecords);

        gannWrapper.getGANN().setMutationRate(mutationRate);
        gannWrapper.getGANN().setCrossoverRate(crossoverRate);
        
        gannWrapper.writeInScore("Configuration\n");
        gannWrapper.writeInScore("---------------------------\n");
        gannWrapper.writeInScore("Generations: " + to_string(generations) + "\n");
        gannWrapper.writeInScore("Population: " + to_string(population) + "\n");
        gannWrapper.writeInScore("Mutation rate: " + to_string(mutationRate) + "\n");
        gannWrapper.writeInScore("Elitism: " + to_string(elitism) + "\n");
        gannWrapper.writeInScore("Weights Factor: " + to_string(weightsFactor) + "\n");
        gannWrapper.writeInScore("Game: Boxing\n");
        gannWrapper.writeInScore("Topology: ");
        UtilG::printVector(topologyBoxing, gannWrapper.getBestFitnessAndScoreFileTxt());
        gannWrapper.writeInScore("\n\n");

        NeuralNetwork nn = gannWrapper.trainAndGetBestNN();

        cout << "Best neural network is playing!" << endl;
        Player::playBoxing(nn, true, steps);
    }
    else if (game == Game::demonAttack)
    {
        double mutationRate = 0.2;
        double crossoverRate = 0.7;
        size_t elitism = 5;
        size_t weightsFactor = 2;
        vector<int> topologyDemonAttack = {3, 3};

        GANNWrapper gannWrapper(topologyDemonAttack, Game::demonAttack, steps, generations, population, elitism, weightsFactor, filenameToSaveRecords);

        gannWrapper.getGANN().setMutationRate(mutationRate);
        gannWrapper.getGANN().setCrossoverRate(crossoverRate);
        
        gannWrapper.writeInScore("Configuration\n");
        gannWrapper.writeInScore("---------------------------\n");
        gannWrapper.writeInScore("Generations: " + to_string(generations) + "\n");
        gannWrapper.writeInScore("Population: " + to_string(population) + "\n");
        gannWrapper.writeInScore("Mutation rate: " + to_string(mutationRate) + "\n");
        gannWrapper.writeInScore("Elitism: " + to_string(elitism) + "\n");
        gannWrapper.writeInScore("Weights Factor: " + to_string(weightsFactor) + "\n");
        gannWrapper.writeInScore("Game: Demon Attack\n");
        gannWrapper.writeInScore("Topology: ");
        UtilG::printVector(topologyDemonAttack, gannWrapper.getBestFitnessAndScoreFileTxt());
        gannWrapper.writeInScore("\n\n");

        NeuralNetwork nn = gannWrapper.trainAndGetBestNN();

        cout << "Best neural network is playing!" << endl;
        Player::playDemonAttack(nn, true, steps);
    }
    else if (game == Game::starGunner)
    {
        double mutationRate = 0.2;
        double crossoverRate = 0.7;
        size_t elitism = 5;
        size_t weightsFactor = 2;
        vector<int> topologyStarGunner = {12, 5};

        GANNWrapper gannWrapper(topologyStarGunner, Game::starGunner, steps, generations, population, elitism, weightsFactor, filenameToSaveRecords);

        gannWrapper.getGANN().setMutationRate(mutationRate);
        gannWrapper.getGANN().setCrossoverRate(crossoverRate);
        
        gannWrapper.writeInScore("Configuration\n");
        gannWrapper.writeInScore("---------------------------\n");
        gannWrapper.writeInScore("Generations: " + to_string(generations) + "\n");
        gannWrapper.writeInScore("Population: " + to_string(population) + "\n");
        gannWrapper.writeInScore("Mutation rate: " + to_string(mutationRate) + "\n");
        gannWrapper.writeInScore("Elitism: " + to_string(elitism) + "\n");
        gannWrapper.writeInScore("Weights Factor: " + to_string(weightsFactor) + "\n");
        gannWrapper.writeInScore("Game: StarGunner\n");
        gannWrapper.writeInScore("Topology: ");
        UtilG::printVector(topologyStarGunner, gannWrapper.getBestFitnessAndScoreFileTxt());
        gannWrapper.writeInScore("\n\n");

        NeuralNetwork nn = gannWrapper.trainAndGetBestNN();

        cout << "Best neural network is playing!" << endl;
        Player::playStarGunner(nn, true, steps);
    }
    else
    {
        //cerr << "ERROR: game unknown." << endl;   // Standard error output is redirected to /dev/null
        //cout << "ERROR: game unknown." << endl;
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