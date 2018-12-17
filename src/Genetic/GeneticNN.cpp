
#include <iostream>

#include "GeneticNN.h"
#include "UtilG.h"
#include "../NeuralNetwork/neuralNetwork.h"
#include "Player.h"

GeneticNN::GeneticNN(const vector<int> &topology, Game game, unsigned population) :
    population(population),
    fitnessValues(population)
{
    this->currentGeneration = 0;
    this->populationSize = population;
    this->topology = topology;
    this->DNASize = 0;
    this->currentGame = game;

    for (size_t i = 0; i < topology.size() - 1; i++)
    {
        this->DNASize += this->topology[i] * this->topology[i + 1];
        this->DNASize += this->topology[i + 1];
    }
}

void GeneticNN::createPopulation()
{
    for (size_t i = 0; i < this->populationSize; i++)
    {
        this->population[i] = DNA(UtilG::getRandomMatrix(1, this->DNASize));
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

void GeneticNN::setMutation(double mutation)
{
    for (size_t i = 0; i < this->population.size(); i++)
    {
        this->population[i].setMutation(mutation);
    }
}

void GeneticNN::computeFitness()
{
    for (size_t i = 0; i < this->populationSize; ++i)
    {
        vector<int> currentFitness = this->fitness(this->population[i]);

        // Modify fitness function HERE!
        
        if (this->fitnessValues[i] == currentFitness[0])
        {
            this->fitnessValues[i] = currentFitness[0] / 2;
        }
        else
        {
            this->fitnessValues[i] = currentFitness[0];   // Score -> It improves in great steps
        }

        //this->fitnessValues[i] = currentFitness[0];   // Score -> It improves in great steps
        //this->fitnessValues[i] = currentFitness[0] * currentFitness[0];   // Score * Score -> 
        //this->fitnessValues[i] = currentFitness[0] + currentFitness[1]; // Score + Steps -> It improves but when it detects a pattern that improves only steps, it doesn't improve the score
        //this->fitnessValues[i] = currentFitness[0] * 10 + currentFitness[1]; // Score * 10 + Steps -> Same that Score + Steps

        cout << "Progress: " << i*100/this->populationSize << "% (" << i << ") - ";

        for (size_t j = 0; j < currentFitness.size(); ++j)
            cout << currentFitness[j] << " ";
        
        cout << endl;
    }
}

DNA GeneticNN::crossover() const
{
    DNA p1 = this->population[this->getRandomMostLikelyGeneIndex()];
    DNA p2 = this->population[this->getRandomMostLikelyGeneIndex()];
    
    return p1.crossover(p2);
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

    for (unsigned i = 0; i < this->populationSize; i++)
    {
        DNA newDNA = this->crossover();

        newDNA.mutate();
        newDNA.mutatePermutation();

        newPopulation[i] = newDNA;
    }

    this->population = newPopulation;
    this->currentGeneration++;
}

unsigned GeneticNN::getRandomMostLikelyGeneIndex() const
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

unsigned GeneticNN::getCurrentGeneration() const
{
    return this->currentGeneration;
}

void GeneticNN::setCurrentGame(Game game)
{
    this->currentGame = game;
}