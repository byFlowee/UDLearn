
#include <iostream>
#include <limits>

#include "GeneticNN.h"
#include "UtilG.h"
#include "../NeuralNetwork/neuralNetwork.h"
#include "Player.h"

GeneticNN::GeneticNN(const vector<int> &topology, Game game, size_t population, size_t bestToAdd, size_t weightsFactor) :
    population(population),
    fitnessValues(population),
    topology(topology),
    DNASize(0),
    currentGame(game),
    bestToAdd(bestToAdd),
    weightsFactor(weightsFactor)
{
    this->currentGeneration = 0;
    this->populationSize = population;

    for (size_t i = 0; i < topology.size() - 1; i++)
    {
        this->DNASize += this->topology[i] * this->topology[i + 1];
        this->DNASize += this->topology[i + 1];
    }
}

void GeneticNN::createPopulation()
{
    this->currentGeneration = 0;
    
    for (size_t i = 0; i < this->populationSize; i++)
    {
        this->population[i] = DNA(UtilG::getRandomMatrix(1, this->DNASize, this->weightsFactor));
    }
}

vector<int> GeneticNN::fitness(const DNA &dna)
{
    //int res = -1;
    vector<int> res;

    NeuralNetwork nn(this->topology);

    vector<vector<Mat>> nnWeightsAndBias = UtilG::setRepresentativeVectorOnNeuralNetwork(dna.getGenes(), nn);

    // Play game and set res to the score
    switch (this->currentGame)
    {
        case Game::breakout:
            res = Player::playBreakout(nn);
            break;
        case Game::boxing:
            res = Player::playBoxing(nn);
            break;
        case Game::demonAttack:
            res = Player::playDemonAttack(nn);
            break;
        case Game::starGunner:
            res = Player::playStarGunner(nn);
            break;
        default:
            cerr << "ERROR: Unknown game." << endl;
    }

    return res;
}

void GeneticNN::setMutationRate(double mutationRate)
{
    for (size_t i = 0; i < this->population.size(); i++)
    {
        this->population[i].setMutationRate(mutationRate);
    }
}

void GeneticNN::setCrossoverRate(double crossoverRate)
{
    for (size_t i = 0; i < this->population.size(); i++)
    {
        this->population[i].setCrossoverRate(crossoverRate);
    }
}

void GeneticNN::computeFitness()
{
    int bestFitness = numeric_limits<int>::min();

    for (size_t i = 0; i < this->populationSize; ++i)
    {
        vector<int> currentFitness = this->fitness(this->population[i]);

        // Modify fitness function HERE!

        //this->fitnessValues[i] = currentFitness[0];   // Score -> It improves in great steps
        //this->fitnessValues[i] = currentFitness[0] * currentFitness[0];   // Score * Score -> 
        //this->fitnessValues[i] = currentFitness[0] + currentFitness[1]; // Score + Steps -> It improves but when it detects a pattern that improves only steps, it doesn't improve the score
        //this->fitnessValues[i] = currentFitness[0] * 10 + currentFitness[1]; // Score * 10 + Steps -> Same that Score + Steps

        switch(this->currentGame)
        {
            case Game::breakout:
                this->fitnessValues[i] = currentFitness[0];   // Score
                break;
            case Game::boxing:
                // TODO
                this->fitnessValues[i] = -666;
                break;
            case Game::demonAttack:
                //this->fitnessValues[i] = currentFitness[0] * ((currentFitness[2] / 100.0) * (currentFitness[3] / 100.0));
                //this->fitnessValues[i] = ((double)(currentFitness[0] * currentFitness[0]) / (500.0 * 500.0)) * ((currentFitness[2] / 1000.0) * (currentFitness[3] / 1000.0));
                //this->fitnessValues[i] = ((double)(currentFitness[0] * currentFitness[0]) / (500.0 * 500.0)) * ((currentFitness[2] / 1000) * (currentFitness[3] / 1000)) * ((currentFitness[0] / 1000) + 1);
                //this->fitnessValues[i] = (currentFitness[0] / 10) * ((currentFitness[2] / 1000) * (currentFitness[3] / 1000));

                //this->fitnessValues[i] = (currentFitness[0] / 10) * ((currentFitness[2] / (int)((currentFitness[1] * 500.0) / 15000.0)) * (currentFitness[3] / (int)((currentFitness[1] * 500.0) / 15000.0)) * (currentFitness[4] / (int)((currentFitness[1] * 1000.0) / 15000.0)));

                this->fitnessValues[i] = (currentFitness[0] / 100) * (currentFitness[0] / 100) * ((currentFitness[2] + currentFitness[3]) / 1000) * (currentFitness[4] / 2000) * (min(currentFitness[2], currentFitness[3]) / 100);

                break;
            case Game::starGunner:
                // TODO
                this->fitnessValues[i] = -666;
                break;
            default:
                cerr << "ERROR: unknown game." << endl;
                return;
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
}

DNA GeneticNN::crossover() const
{
    size_t index1 = this->getRandomMostLikelyGeneIndex();
    size_t index2 = this->getRandomMostLikelyGeneIndex();

    DNA p1 = this->population[index1];
    DNA p2 = this->population[index2];
    
    return p1.crossover(p2, this->fitnessValues[index1], this->fitnessValues[index2]);
}

DNA GeneticNN::getCurrentBestDNA() const
{
    int best = -1;
    int index = 0;

    for (size_t i = 0; i < this->fitnessValues.size(); i++)
    {
        if (this->fitnessValues[i] > best)
        {
            best = this->fitnessValues[i];
            index = i;
        }
    }

    return this->population[index];
}

int GeneticNN::getCurrentBestDNAFitness() const
{
    int best = -1;
    int index = 0;

    for (size_t i = 0; i < this->fitnessValues.size(); i++)
    {
        if (this->fitnessValues[i] > best)
        {
            best = this->fitnessValues[i];
            index = i;
        }
    }

    return this->fitnessValues[index];
}

void GeneticNN::nextGeneration()
{
    // We need a new vector because we can't rewrite directly on this->population
    //  due to the fact that we need the original population to get the new population.
    vector<DNA> newPopulation(this->populationSize);

    // Elitism.
    DNA bestDNA = getCurrentBestDNA();
    
    for (size_t i = 0; i < min(this->bestToAdd, this->populationSize); i++)
    {
        newPopulation[i] = bestDNA;
    }

    for (size_t i = this->bestToAdd; i < this->populationSize; i++)
    {
        DNA newDNA = this->crossover();

        newDNA.mutate(this->weightsFactor);
        newDNA.mutatePermutation();

        newPopulation[i] = newDNA;
    }

    this->population = newPopulation;
    this->currentGeneration++;
}

size_t GeneticNN::getRandomMostLikelyGeneIndex() const
{
    int sum = 0;

    for (size_t i = 0; i < this->fitnessValues.size(); i++)
    {
        sum += this->fitnessValues[i];
    }

    // sum = total fitness

    int index = 0;
    double random = UtilG::getRandomDouble(0.0, 1.0);

    for (size_t i = 0; i < this->fitnessValues.size(); i++)
    {
        random -= (double)this->fitnessValues[i] / (double)sum;     // random -= fitness normalized

        if (random < 0.0)
        {
            index = i;
            break;
        }
    }

    return index;
}

size_t GeneticNN::getCurrentGeneration() const
{
    return this->currentGeneration;
}

void GeneticNN::setCurrentGame(Game game)
{
    this->currentGame = game;
}