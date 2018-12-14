#ifndef _GeneticNN_
#define _GeneticNN_

#include "DNA.h"

class GeneticNN {
    private:
        unsigned totalPopulation;
        vector<DNA> population;
        vector<int> fitnessValues;
        unsigned rows;
        unsigned cols;
        unsigned maxGenerations;
        unsigned generation;
        int fitness();
        unsigned getRandomMostLikelyGeneIndex();

    public:
        GeneticNN(const vector<unsigned> &topology, unsigned = 0, unsigned = 50);
        void createPopulation();
        void computeFitness();
        DNA crossover();
        void getCurrentBest();
        void mutate(DNA);
        void newGeneration();

};

#endif