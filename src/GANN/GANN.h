#ifndef _GANN_
#define _GANN_

#include <string>

#include "DNA.h"

enum Game
{
    breakout = 1,
    boxing = 2,
    demonAttack = 3,
    starGunner = 4
};

struct WeightInitializationRange
{
    private:
        double left;
        double right;

    public:
        WeightInitializationRange()
        {
            this->left = -1.0;
            this->right = 1.0;
        }

        WeightInitializationRange(double left, double right)
        {
            if (left <= right)
            {
                this->left = left;
                this->right = right;
            }
            else
            {
                this->left = -1.0;
                this->right = 1.0;
            }
        }

        double getLeft() const
        {
            return this->left;
        }

        double getRight() const
        {
            return this->right;
        }
};

class GANN
{
    
    private:
        size_t populationSize;
        vector<DNA> population;
        vector<double> fitnessValues;
        vector<double> initialElitism;
        vector<WeightInitializationRange> weightsInitializationRange;
        vector<int> topology;
        size_t currentGeneration;
        size_t DNASize;
        Game currentGame;
        // Elitism.
        size_t elitism;
        size_t weightsFactor;

        vector<int> fitness(const DNA&);
        size_t getRandomMostLikelyGeneIndex() const;
        double getFitnessValue(const vector<int>&);

    public:
        GANN(const vector<int>&, Game, const vector<double>&, const vector<WeightInitializationRange>&, size_t = 50, size_t = 0, size_t = 1);
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
        void setWeightInitializationRange(const vector<WeightInitializationRange>&);
        void setInitialElitism(const vector<double>&);

};

#endif