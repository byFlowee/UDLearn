#ifndef _SPECIES_
#define _SPECIES_

#include "Genome.h"

class Species {
    private:
        Genome best;
        vector<Genome*> members;
        unsigned specieID;
        double bestFitness;
        unsigned gensWithoutImprove;
        unsigned age;

        double younThreshold;
        double youngBonus;
        double oldThreshold;
        double oldPenalty;

    public:
        Species(Genome &base, unsigned id);
        void adjustFitness();
        void addMember(Genome &newMember);

        Genome getBest() { return this->best; }

};

#endif