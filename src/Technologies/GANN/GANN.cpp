
#include <iostream>
#include <limits>

#include "GANN.h"
#include "UtilG.h"
#include "../NeuralNetwork/neuralNetwork.h"
#include "Player.h"

GANN::GANN(const vector<int> &topology, Game game, int gameSteps, const vector<double> &initialElitism, const vector<WeightInitializationRange> &weightsInitializationRange, size_t population, size_t elitism, size_t weightsFactor) :
    population(population),
    fitnessValues(population),
    topology(topology),
    DNASize(0),
    currentGame(game),
    elitism(elitism),
    weightsFactor(weightsFactor)
{
    this->currentGeneration = 0;
    this->gameSteps = gameSteps;
    this->populationSize = population;

    if (this->weightsFactor <= 0)
    {
        this->weightsFactor = 1;
    }

    for (size_t i = 0; i < topology.size() - 1; i++)
    {
        this->DNASize += this->topology[i] * this->topology[i + 1];
        this->DNASize += this->topology[i + 1];
    }

    this->setInitialElitism(initialElitism);
    this->setWeightInitializationRange(weightsInitializationRange);
}

void GANN::setWeightInitializationRange(const vector<WeightInitializationRange> &weightsInitializationRange)
{
    this->weightsInitializationRange = weightsInitializationRange;

    if (this->weightsInitializationRange.empty() || this->weightsInitializationRange.size() != this->DNASize)
    {
        this->weightsInitializationRange.resize(this->DNASize);

        WeightInitializationRange range(-1.0 * (double)this->weightsFactor, 1.0 * (double)this->weightsFactor);

        for (size_t i = 0; i < this->weightsInitializationRange.size(); i++)
        {
            this->weightsInitializationRange[i] = range;
        }
    }
}

void GANN::setInitialElitism(const vector<double> &initialElitism)
{
    this->initialElitism = initialElitism;
}

void GANN::createPopulation()
{
    this->currentGeneration = 0;
    
    if (!this->initialElitism.empty() && this->initialElitism.size() == this->DNASize)
    {
        Mat elitismInitialization(1, this->DNASize);

        for (size_t i = 0; i < this->DNASize; i++)
        {
            elitismInitialization.set(0, i, this->initialElitism[i]);
        }

        for (size_t i = 0; i < min(this->elitism, this->populationSize); i++)
        {
            this->population[i] = DNA(elitismInitialization);
        }

        for (size_t i = this->elitism; i < this->populationSize; i++)
        {
            //this->population[i] = DNA(UtilG::getRandomMatrix(1, this->DNASize, this->weightsFactor));
            this->population[i] = DNA(UtilG::getRandomMatrix(1, this->DNASize, this->weightsInitializationRange));
        }
    }
    else
    {
        for (size_t i = 0; i < this->populationSize; i++)
        {
            //this->population[i] = DNA(UtilG::getRandomMatrix(1, this->DNASize, this->weightsFactor));
            this->population[i] = DNA(UtilG::getRandomMatrix(1, this->DNASize, this->weightsInitializationRange));
        }
    }
}

vector<int> GANN::fitness(const DNA &dna)
{
    //int res = -1;
    vector<int> res;

    NeuralNetwork nn(this->topology);

    vector<vector<Mat>> nnWeightsAndBias = UtilG::setRepresentativeVectorOnNeuralNetwork(dna.getGenes(), nn);

    // Play game and set res to the score
    switch (this->currentGame)
    {
        case Game::breakout:
            res = Player::playBreakout(nn, false, this->gameSteps);
            break;
        case Game::boxing:
            res = Player::playBoxing(nn, false, this->gameSteps);
            break;
        case Game::demonAttack:
            res = Player::playDemonAttack(nn, false, this->gameSteps);
            break;
        case Game::starGunner:
            res = Player::playStarGunner(nn, false, this->gameSteps);
            break;
        default:
            cerr << "ERROR: Unknown game." << endl;
    }

    return res;
}

void GANN::setMutationRate(double mutationRate)
{
    for (size_t i = 0; i < this->population.size(); i++)
    {
        this->population[i].setMutationRate(mutationRate);
    }
}

void GANN::setCrossoverRate(double crossoverRate)
{
    for (size_t i = 0; i < this->population.size(); i++)
    {
        this->population[i].setCrossoverRate(crossoverRate);
    }
}

void GANN::fitnessSharing()
{
    const int scoreDivision = 5;
    const int stepsDivision = 5;
    int maxScore = numeric_limits<int>::min();
    int maxSteps = numeric_limits<int>::min();
    vector<int> nicheID(this->populationSize);
    vector<int> sumSameNicheID(scoreDivision * stepsDivision, 0);
    vector<int> allScores;
    vector<int> allSteps;
    double bestFitness = numeric_limits<double>::lowest();

    cout << endl;
    cout << "   Fitness Sharing" << endl;
    cout << "   ---------------" << endl;

    for (size_t i = 0; i < this->populationSize; i++)
    {
        vector<int> currentFitness = this->fitness(this->population[i]);
        int currentScore = currentFitness[0];
        int currentSteps = currentFitness[1];

        this->fitnessValues[i] = this->getFitnessValue(currentFitness);

        allScores.push_back(currentScore);
        allSteps.push_back(currentSteps);

        if (currentScore > maxScore)
        {
            maxScore = currentScore;
        }
        if (currentSteps > maxSteps)
        {
            maxSteps = currentSteps;
        }

        cout << " - Progress: " << i * 100 / this->populationSize << "% (" << i << ")\t- ";

        if (this->fitnessValues[i] > bestFitness)
        {
            bestFitness = this->fitnessValues[i];

            cout << "\033[1;32m" << this->fitnessValues[i] << "\033[0m\t- ";
        }
        else
        {
            cout << this->fitnessValues[i] << "\t- ";
        }

        for (size_t j = 0; j < currentFitness.size(); ++j)
            cout << currentFitness[j] << " ";
        
        cout << endl;
    }

    cout << endl;
    cout << "   Niche ID Summary" << endl;
    cout << "   ----------------" << endl;

    for (size_t i = 0; i < this->populationSize; i++)
    {
        int row = 0;

        for (int j = 1; j <= scoreDivision; j++)
        {
            if ((double)(maxScore * j) / (double)scoreDivision >= (double)allScores[i])
            {
                row = j;
                break;
            }
        }

        int col = 0;

        for (int j = 1; j <= stepsDivision; j++)
        {
            if ((double)(maxSteps * j) / (double)stepsDivision >= (double)allSteps[i])
            {
                col = j;
                break;
            }
        }

        int currentNicheID = (row - 1) * stepsDivision + col;

        sumSameNicheID[currentNicheID - 1]++;
        nicheID[i] = currentNicheID;

        cout << " - [Score, Steps, Row, Col, currentNicheID] = [" << allScores[i] << ", " << allSteps[i] << ", " << row << ", " << col << ", " << currentNicheID << "]" << endl;
    }

    cout << endl;
    cout << "   Fitness Update - Sharing Fitness" << endl;
    cout << "   --------------------------------" << endl;
    
    for (size_t i = 0; i < this->populationSize; i++)
    {
        double fitnessSharingValue = this->fitnessValues[i] / (double)sumSameNicheID[nicheID[i] - 1];

        cout << " - [Not Sharing Fitness -> Sharing Fitness] = [" << this->fitnessValues[i] << " -> " << fitnessSharingValue << "]" << endl;

        this->fitnessValues[i] = fitnessSharingValue;
    }

    cout << "   -------------------" << endl;
    cout << "   End Fitness Sharing" << endl;
    cout << endl;
}

double GANN::getFitnessValue(const vector<int> &currentFitness)
{
    // Modify fitness function HERE!

    double fitness = 0.0;
    int score = currentFitness[0];
    int steps = currentFitness[1];

    switch(this->currentGame)
    {
        case Game::breakout:
            fitness = score;
            break;
        case Game::boxing:
            //fitness = currentFitness[0] + (double)currentFitness[2] * ((double)currentFitness[2] / (double)(currentFitness[3] + 1));
            
            if (score < 0)
            {
                fitness = score * (1 - (double)currentFitness[4] / (double)steps) * (1 - (double)currentFitness[5] / (double)steps) * (1 - (double)currentFitness[6] / (double)steps) * (1 - (double)currentFitness[7] / (double)steps) + (double)currentFitness[2] * ((double)currentFitness[2] / (double)(currentFitness[3] + 1));
            }
            else
            {
                fitness = score * ((double)currentFitness[4] / (double)steps) * ((double)currentFitness[5] / (double)steps) * ((double)currentFitness[6] / (double)steps) * ((double)currentFitness[7] / (double)steps) + (double)currentFitness[2] * ((double)currentFitness[2] / (double)(currentFitness[3] + 1));
            }
            break;
        case Game::demonAttack:
            //fitness = currentFitness[0] * ((currentFitness[2] / 100.0) * (currentFitness[3] / 100.0));
            //fitness = ((double)(currentFitness[0] * currentFitness[0]) / (500.0 * 500.0)) * ((currentFitness[2] / 1000.0) * (currentFitness[3] / 1000.0));
            //fitness = ((double)(currentFitness[0] * currentFitness[0]) / (500.0 * 500.0)) * ((currentFitness[2] / 1000) * (currentFitness[3] / 1000)) * ((currentFitness[0] / 1000) + 1);
            //fitness = (currentFitness[0] / 10) * ((currentFitness[2] / 1000) * (currentFitness[3] / 1000));

            //fitness = (currentFitness[0] / 10) * ((currentFitness[2] / (int)((currentFitness[1] * 500.0) / 15000.0)) * (currentFitness[3] / (int)((currentFitness[1] * 500.0) / 15000.0)) * (currentFitness[4] / (int)((currentFitness[1] * 1000.0) / 15000.0)));

            //fitness = (currentFitness[0] / 100) * (currentFitness[0] / 100) * ((currentFitness[2] + currentFitness[3]) / 1000) * (currentFitness[4] / 2000) * (min(currentFitness[2], currentFitness[3]) / 100);

            //fitness = (currentFitness[0] / 10) * (currentFitness[4] / 500) * (min(currentFitness[2], currentFitness[3]) / 100);

            //fitness = (currentFitness[0] / 10) * (currentFitness[4] / 500) * (min(currentFitness[2], currentFitness[3]) / 100);;

            //fitness = (currentFitness[0] / 10) * (currentFitness[0] / 10) * (min(currentFitness[2], currentFitness[3]) / 100);

            /*
            if (min(currentFitness[2], currentFitness[3]) > 100)
            {
                fitness = currentFitness[0];
            }
            else
            {
                fitness = 0.0;
            }
            */

            if (min(currentFitness[2], currentFitness[3]) != 0)
            {
                double punch = (double)currentFitness[5] / (double)steps;
                double dscore = (double)score;

                punch *= punch;
                dscore /= 10;

                fitness = dscore * punch;
            }
            else
            {
                fitness = 0.0;
            }

            break;
        case Game::starGunner:
            fitness = score / 100;
            break;
        default:
            //cerr << "ERROR: game unknown." << endl;   // Standard error output is redirected to /dev/null
            cout << "ERROR: game unknown." << endl;
    }

    return fitness;
}

void GANN::computeFitness()
{
    double bestFitness = numeric_limits<double>::lowest();

    for (size_t i = 0; i < this->populationSize; ++i)
    {
        vector<int> currentFitness = this->fitness(this->population[i]);

        this->fitnessValues[i] = this->getFitnessValue(currentFitness);

        cout << " - Progress: " << i * 100 / this->populationSize << "% (" << i << ")\t- ";

        if (this->fitnessValues[i] > bestFitness)
        {
            bestFitness = this->fitnessValues[i];

            cout << "\033[1;32m" << this->fitnessValues[i] << "\033[0m\t- ";
        }
        else
        {
            cout << this->fitnessValues[i] << "\t- ";
        }

        for (size_t j = 0; j < currentFitness.size(); ++j)
            cout << currentFitness[j] << " ";
        
        cout << endl;
    }
}

DNA GANN::crossover() const
{
    size_t index1 = this->getRandomMostLikelyGeneIndex();
    size_t index2 = this->getRandomMostLikelyGeneIndex();

    DNA p1 = this->population[index1];
    DNA p2 = this->population[index2];
    
    return p1.crossover(p2, this->fitnessValues[index1], this->fitnessValues[index2]);
}

DNA GANN::getCurrentBestDNA() const
{
    double bestFitness = numeric_limits<double>::lowest();
    int index = 0;

    for (size_t i = 0; i < this->fitnessValues.size(); i++)
    {
        if (this->fitnessValues[i] > bestFitness)
        {
            bestFitness = this->fitnessValues[i];
            index = i;
        }
    }

    return this->population[index];
}

double GANN::getCurrentBestDNAFitness() const
{
    double bestFitness = numeric_limits<double>::lowest();
    int index = 0;

    for (size_t i = 0; i < this->fitnessValues.size(); i++)
    {
        if (this->fitnessValues[i] > bestFitness)
        {
            bestFitness = this->fitnessValues[i];
            index = i;
        }
    }

    return this->fitnessValues[index];
}

void GANN::nextGeneration()
{
    // We need a new vector because we can't rewrite directly on this->population
    //  due to the fact that we need the original population to get the new population.
    vector<DNA> newPopulation(this->populationSize);

    // Elitism.
    DNA bestDNA = getCurrentBestDNA();
    
    for (size_t i = 0; i < min(this->elitism, this->populationSize); i++)
    {
        newPopulation[i] = bestDNA;
    }

    for (size_t i = this->elitism; i < this->populationSize; i++)
    {
        DNA newDNA = this->crossover();

        newDNA.mutate(this->weightsFactor);
        newDNA.mutatePermutation();

        newPopulation[i] = newDNA;
    }

    this->population = newPopulation;
    this->currentGeneration++;
}

size_t GANN::getRandomMostLikelyGeneIndex() const
{
    double sum = 0.0;

    for (size_t i = 0; i < this->fitnessValues.size(); i++)
    {
        sum += this->fitnessValues[i];
    }

    // sum = total fitness

    int index = 0;
    double random = UtilG::getRandomDouble(0.0, 1.0);

    for (size_t i = 0; i < this->fitnessValues.size(); i++)
    {
        random -= this->fitnessValues[i] / sum;     // random -= fitness normalized

        if (random < 0.0)
        {
            index = i;
            break;
        }
    }

    return index;
}

size_t GANN::getCurrentGeneration() const
{
    return this->currentGeneration;
}

void GANN::setCurrentGame(Game game)
{
    this->currentGame = game;
}