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
        size_t populationSize;
        vector<DNA> population;
        vector<double> fitnessValues;
        vector<int> topology;
        size_t currentGeneration;
        size_t DNASize;
        Game currentGame;
        // Elitism.
        size_t bestToAdd;
        size_t weightsFactor;

        vector<int> fitness(const DNA&);
        size_t getRandomMostLikelyGeneIndex() const;
        double getFitnessValue(const vector<int>&);

    public:
        GeneticNN(const vector<int> &topology, Game, size_t = 50, size_t = 0, size_t = 1);
        void setMutationRate(double);
        void setCrossoverRate(double);
        void createPopulation();
        void computeFitness();
        void fitnessSharing();
        DNA crossover() const;
        DNA getCurrentBestDNA() const;
        double getCurrentBestDNAFitness() const;
        void nextGeneration();
        size_t getCurrentGeneration() const;
        void setCurrentGame(Game);

};

#endif