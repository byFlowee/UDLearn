#ifndef _GeneticNN_
#define _GeneticNN_

#include "ADN.h"

class GeneticNN {
    private:
        unsigned totalPopulation;
        vector<ADN> population;
        vector<int> fitnessValues;
        unsigned rows;
        unsigned cols;
        unsigned maxGenerations;
        unsigned generation;
        int fitness();
        unsigned getRandomMostLikelyGeneIndex();

    public:
        GeneticNN(unsigned, unsigned, const vector<unsigned> &topology);
        void createPopulation();
        void computeFitness();
        ADN crossover();
        void getCurrentBest();
        void mutate(ADN);
        void newGeneration();

};

#endif