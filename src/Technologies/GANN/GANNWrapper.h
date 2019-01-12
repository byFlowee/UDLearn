
#ifndef __GANNWRAPPER__
#define __GANNWRAPPER__

#include <fstream>
#include <vector>

#include "GANN.h"
#include "../NeuralNetwork/neuralNetwork.h"

class GANNWrapper
{
    private:
        GANN gann;
        int generations;
        ofstream bestFitnessAndScoreFileTxt;
        ofstream bestFitnessAndScoreFileWeightsAndBiases;
        vector<int> topology;
        bool fitnessSharing;
        Game game;
        int gameSteps;

    public:
        GANNWrapper(const vector<int>&, Game, int, int, int, size_t, size_t, const string&);
        ~GANNWrapper();
        GANN& getGANN();
        int getGenerations() const;
        vector<int> getTopology() const;
        void setGenerations(int);
        void setFitnessSharing(bool);
        void writeInScore(const string&);
        ofstream& getBestFitnessAndScoreFileTxt();
        NeuralNetwork trainAndGetBestNN();

};

#endif