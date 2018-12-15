#ifndef _GeneticNN_
#define _GeneticNN_

#include "DNA.h"

class GeneticNN {
    private:
        unsigned populationSize;
        vector<DNA> population;
        vector<int> fitnessValues;
        vector<unsigned> topology;
        unsigned maxGenerations;
        unsigned currentGeneration;
        unsigned DNASize;

        int fitness();
        unsigned getRandomMostLikelyGeneIndex() const;

    public:
        GeneticNN(const vector<unsigned> &topology, unsigned = 0, unsigned = 50);
        void createPopulation();
        void computeFitness();
        DNA crossover() const;
        DNA getCurrentBestDNA() const;
        void nextGeneration();
        unsigned getCurrentGeneration() const;

};

#endif