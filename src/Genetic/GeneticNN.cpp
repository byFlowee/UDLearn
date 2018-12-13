#include "GeneticNN.h"
#include "UtilG.h"

GeneticNN::GeneticNN(unsigned maxGenerations = 0, unsigned population = 50, const vector<unsigned> &topology) :
    population(population),
    fitnessValues(population)
{
    this->generation = 0;
    this->maxGenerations = maxGenerations;
    this->totalPopulation = population;
}

void GeneticNN::createPopulation() {
    for (int i = 0; i < this->totalPopulation; i++)
    {
        this->population[i] = ADN(UtilG::getRandomMatrix(this->rows*this->cols,1));
    }                    
}

void GeneticNN::computeFitness() {
    for (int i = 0; i < this->population.size(); ++i) {
        this->fitnessValues[i] = this->fitness(); 
    }
}

ADN GeneticNN::crossover() {
    ADN p1 = this->population[this->getRandomMostLikelyGeneIndex()];
    ADN p2 = this->population[this->getRandomMostLikelyGeneIndex()];
    
    return p1.crossover(p2);
}

void GeneticNN::getCurrentBest() {
    
    return;
}

void GeneticNN::mutate(ADN a) {
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

    for (int i = 0; i < this->fitnessValues.size(); ++i)

    return 0;
}
