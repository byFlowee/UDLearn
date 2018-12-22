#ifndef _DNA_
#define _DNA_

#include <vector>
#include <string>

#include "../NeuralNetwork/mat.h"

using namespace std;

class DNA {
    private:
        double mutationRate;
        double crossoverRate;
        Mat genes;

    public:
        friend bool operator== (const DNA&, const DNA&);
        DNA& operator=(const DNA&);

        DNA(int = 1, int = 1);
        DNA(const Mat&, double = 0.01, double = 0.7);
        void setMutationRate(double);
        void setCrossoverRate(double);
        DNA crossover(const DNA&, int, int);
        void mutate(size_t = 1);
        void mutatePermutation();
        Mat getGenes() const;
};

#endif