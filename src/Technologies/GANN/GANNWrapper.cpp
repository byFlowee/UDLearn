
#include <iostream>
#include <limits>

#include "GANNWrapper.h"
#include "Player.h"
#include "UtilG.h"

using std::cout;

GANNWrapper::GANNWrapper(const vector<int> &topology, Game game, int gameSteps, int generations, int population, size_t elitism, size_t weightsFactor, const string &filename) :
    gann(topology, game, gameSteps, {}, {}, population, elitism, weightsFactor),
    generations(generations),
    bestFitnessAndScoreFileTxt(("records/" + filename + ".txt").c_str()),
    bestFitnessAndScoreFileWeightsAndBiases(("records/" + filename + ".weights").c_str()),
    topology(topology),
    fitnessSharing(false),
    game(game),
    gameSteps(gameSteps)
{
    if (!this->bestFitnessAndScoreFileTxt.is_open() || !this->bestFitnessAndScoreFileWeightsAndBiases.is_open())
    {
        //cerr << "..."; // Standard error output is redirected to /dev/null
        cout << "ERROR: couldn't open the file to save the records." << endl;
        cout << "Expected path: ./records/" << filename << ".txt|.weights" << endl;

        throw;
    }
}

GANNWrapper::~GANNWrapper()
{
    if (this->bestFitnessAndScoreFileTxt.is_open())
    {
        this->bestFitnessAndScoreFileTxt.close();
    }
    if (this->bestFitnessAndScoreFileWeightsAndBiases.is_open())
    {
        this->bestFitnessAndScoreFileWeightsAndBiases.close();
    }
}

ofstream& GANNWrapper::getBestFitnessAndScoreFileTxt()
{
    return this->bestFitnessAndScoreFileTxt;
}

GANN& GANNWrapper::getGANN()
{
    return this->gann;
}

int GANNWrapper::getGenerations() const
{
    return this->generations;
}

vector<int> GANNWrapper::getTopology() const
{
    return this->topology;
}

void GANNWrapper::setGenerations(int generations)
{
    this->generations = generations;
}

void GANNWrapper::setFitnessSharing(bool fitnessSharing)
{
    this->fitnessSharing = fitnessSharing;
}

void GANNWrapper::writeInScore(const string &message)
{
    this->bestFitnessAndScoreFileTxt << message;
}

NeuralNetwork GANNWrapper::trainAndGetBestNN()
{
    int score = 0;
    int steps = 0;
    NeuralNetwork nn(this->topology);
    vector<vector<Mat>> weightsAndBias;
    DNA bestDNABasedOnScore;
    DNA bestDNABasedOnFitness;
    int bestScore = numeric_limits<int>::min();
    double bestFitness = numeric_limits<double>::lowest();
    double fitnessValue = 0.0;
    bool improvedScore = true;
    bool improvedFitness = true;

    this->gann.createPopulation();
    //this->gann.setMutationRate(mutationRate);

    for (int i = 0; i < this->generations; i++)
	{
        
        if (this->fitnessSharing)
        {
            this->gann.fitnessSharing();
        }
        else
        {
            this->gann.computeFitness();
        }

        double fitnessAux = this->gann.getCurrentBestDNAFitness();

        if (fitnessAux > fitnessValue)
        {
            improvedFitness = true;
        }
        else
        {
            improvedFitness = false;
        }
			
        fitnessValue = fitnessAux;
        DNA currentBest = this->gann.getCurrentBestDNA();
        Mat genes = currentBest.getGenes();
        vector<int> playerResults;

        // Weights and bias of the best Neural Network
        weightsAndBias = UtilG::setRepresentativeVectorOnNeuralNetwork(genes, nn);

        //score = Player::playBreakout(nn);
        //playerResults = Player::playBreakout(nn, true);   // If you want to see the best DNA of the generation. Can't be closed and continue.. It has to end.

        switch (this->game)
        {
            case Game::breakout:
                playerResults = Player::playBreakout(nn, false, this->gameSteps);           // If you don't want to see the best DNA.
                break;
            case Game::boxing:
                playerResults = Player::playBoxing(nn, false, this->gameSteps);           // If you don't want to see the best DNA.
                break;
            case Game::demonAttack:
                playerResults = Player::playDemonAttack(nn, false, this->gameSteps);           // If you don't want to see the best DNA.
                break;
            case Game::starGunner:
                playerResults = Player::playStarGunner(nn, false, this->gameSteps);           // If you don't want to see the best DNA.
                break;
            default:
                //cerr << "ERROR: game unknown." << endl;   // Standard error output is redirected to /dev/null
                cout << "ERROR: game unknown." << endl;
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

        //cout << "Generation " << this->gann.getCurrentGeneration() << ": " << best << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "------------------- Generation " << this->gann.getCurrentGeneration() + 1 << endl;

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

        this->bestFitnessAndScoreFileTxt << "Generation " << (i + 1) << endl;
        this->bestFitnessAndScoreFileTxt << "---------------------------" << endl;
        this->bestFitnessAndScoreFileTxt << "  Fitness: " << fitnessValue << endl;
        this->bestFitnessAndScoreFileTxt << "  Score: " << score << endl;
        this->bestFitnessAndScoreFileTxt << endl;

        cout << "------------------- Steps playing = " << steps << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << endl;

        if (bestScore < score)
        {
            bestScore = score;
            bestDNABasedOnScore = currentBest;
        }
        if (fitnessValue > bestFitness)
        {
            bestFitness = fitnessValue;
            bestDNABasedOnFitness = currentBest;
        }

        //if (game == Game::breakout && score == 864)
        //{
        //    cout << "MAX SCORE IN BREAKOUT: 864!" << endl;
        //    break;
        //}

        this->gann.nextGeneration();
    }

    Mat flattenedWeightsAndBiasBestBasedOnScore = bestDNABasedOnScore.getGenes();
    Mat flattenedWeightsAndBiasBestBasedOnFitness = bestDNABasedOnFitness.getGenes();

    //cout << "Weights and Bias flattened of the best DNA found (score = " << bestScore << "): ";
    //flattenedWeightsAndBiasBest.print(", ");
    //cout << endl << endl;
    cout << endl;

    this->bestFitnessAndScoreFileWeightsAndBiases << "Best SCORE found: " << bestScore << endl;
    this->bestFitnessAndScoreFileWeightsAndBiases << "Weights and biases" << endl;
    this->bestFitnessAndScoreFileWeightsAndBiases << "---------------------------" << endl;
    flattenedWeightsAndBiasBestBasedOnScore.print(this->bestFitnessAndScoreFileWeightsAndBiases, ", ");
    this->bestFitnessAndScoreFileWeightsAndBiases << endl << endl;
    this->bestFitnessAndScoreFileWeightsAndBiases << "Best FITNESS found: " << bestFitness << endl;
    this->bestFitnessAndScoreFileWeightsAndBiases << "Weights and biases" << endl;
    this->bestFitnessAndScoreFileWeightsAndBiases << "---------------------------" << endl;
    flattenedWeightsAndBiasBestBasedOnFitness.print(this->bestFitnessAndScoreFileWeightsAndBiases, ", ");
    this->bestFitnessAndScoreFileWeightsAndBiases << endl;

    cout << "Returning best DNA based on SCORE" << endl << endl;
    UtilG::setRepresentativeVectorOnNeuralNetwork(flattenedWeightsAndBiasBestBasedOnScore, nn);

    return nn;
}