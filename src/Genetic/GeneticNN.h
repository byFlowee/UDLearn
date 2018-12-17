#ifndef _GeneticNN_
#define _GeneticNN_

#include <string>

#include "DNA.h"

enum Game
{
    breakout = 1,
    boxing = 2,
    demonAttack = 3,
    starGunner = 4
};

class GeneticNN {
    private:
        unsigned populationSize;
        vector<DNA> population;
        vector<int> fitnessValues;
        vector<int> topology;
        unsigned maxGenerations;
        unsigned currentGeneration;
        unsigned DNASize;
        Game currentGame;

        vector<int> fitness(const DNA&);
        unsigned getRandomMostLikelyGeneIndex() const;

    public:
        GeneticNN(const vector<int> &topology, Game, unsigned = 0, unsigned = 50);
        void setMutation(double);
        void createPopulation();
        void computeFitness();
        DNA crossover() const;
        DNA getCurrentBestDNA() const;
        int getCurrentBestDNAFitness() const;
        void nextGeneration();
        unsigned getCurrentGeneration() const;
        void setCurrentGame(Game);

};

#endif