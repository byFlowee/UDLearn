#include "GeneticNN.h"
#include "UtilG.h"

GeneticNN::GeneticNN(const vector<unsigned> &topology, unsigned maxGenerations, unsigned population) :
    population(population),
    fitnessValues(population)
{
    this->currentGeneration = 0;
    this->maxGenerations = maxGenerations;
    this->populationSize = population;
    this->topology = topology;
    this->DNASize = 0;

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

int GeneticNN::fitness()
{
    //TODO
    return 0;
}

void GeneticNN::computeFitness()
{
    for (size_t i = 0; i < this->populationSize; ++i)
    {
        this->fitnessValues[i] = this->fitness(); 
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

void GeneticNN::nextGeneration()
{
    // We need a new vector because we can't rewrite directly on this->population
    //  due to the fact that we need the original population to get the new population.
    vector<DNA> newPopulation(this->populationSize);

    for (unsigned i = 0; i < this->populationSize; i++)
    {
        DNA newDNA = this->crossover();

        newDNA.mutate();

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