#ifndef _GeneticNN_
#define _GeneticNN_

#include <string>

#include "DNA.h"

class GeneticNN {
    private:

        enum Game
        {
            breakout = 1
        };

        unsigned populationSize;
        vector<DNA> population;
        vector<int> fitnessValues;
        vector<int> topology;
        unsigned maxGenerations;
        unsigned currentGeneration;
        unsigned DNASize;

        static Game currentGame;

        vector<int> fitness(const DNA&);
        unsigned getRandomMostLikelyGeneIndex() const;

    public:
        GeneticNN(const vector<int> &topology, unsigned = 0, unsigned = 50);
        void createPopulation();
        void computeFitness();
        DNA crossover() const;
        DNA getCurrentBestDNA() const;
        int getCurrentBestDNAFitness() const;
        void nextGeneration();
        unsigned getCurrentGeneration() const;

};

#endif