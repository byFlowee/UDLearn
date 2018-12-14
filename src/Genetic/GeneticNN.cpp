#include "GeneticNN.h"
#include "UtilG.h"

GeneticNN::GeneticNN(const vector<unsigned> &topology, unsigned maxGenerations, unsigned population) :
    population(population),
    fitnessValues(population)
{
    this->generation = 0;
    this->maxGenerations = maxGenerations;
    this->totalPopulation = population;
}

void GeneticNN::createPopulation() {
    for (size_t i = 0; i < this->totalPopulation; i++)
    {
        this->population[i] = DNA(UtilG::getRandomMatrix(this->rows*this->cols,1));
    }                    
}

void GeneticNN::computeFitness() {
    for (size_t i = 0; i < this->population.size(); ++i) {
        this->fitnessValues[i] = this->fitness(); 
    }
}

DNA GeneticNN::crossover() {
    DNA p1 = this->population[this->getRandomMostLikelyGeneIndex()];
    DNA p2 = this->population[this->getRandomMostLikelyGeneIndex()];
    
    return p1.crossover(p2);
}

void GeneticNN::getCurrentBest() {
    
    return;
}

void GeneticNN::mutate(DNA a) {
    a.mutate();
}

void GeneticNN::newGeneration() {

}

int GeneticNN::fitness() {
    //TODO
    return 0;
}   

unsigned GeneticNN::getRandomMostLikelyGeneIndex() {
    int sum = 0;

    //for (size_t i = 0; i < this->fitnessValues.size(); ++i)

    return 0;
}
