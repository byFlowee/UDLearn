#ifndef _NEAT_
#define _NEAT_

#include <vector>
#include "Genome.h"
using namespace std;

class Neat {
    private:
        vector<Genome> genomes;
        unsigned nextGenomeId;
        bool alreadyAdded(unsigned id, vector<unsigned> &v);
        vector<Net*> Epoch(const vector<double> &fitnessScores);
        Innovation* inn;

    public:
        Genome crossover(Genome &p1, Genome &p2);

};

#endif