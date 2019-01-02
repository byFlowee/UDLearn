#ifndef _GENOME_
#define _GENOME_

#include "Connection.h"
#include "Neuron.h"
#include "Innovation.h"
#include "Net.h"

#include <vector>
#include <cstdlib>

using namespace std;

class Genome {

	public:
		vector<Neuron> neurons;
		vector<Connection> connections;

		Genome(unsigned id, vector<Neuron>, vector<Connection>, unsigned nInputs, unsigned nOutputs);
		Genome() : id(id), nInputs(0), nOutputs(0), phenotype(NULL) {}
		~Genome();
		Genome(const Genome& g);
		Genome& operator=(const Genome &g);
		Net* createPhenotype(unsigned depth);
		void deletePhenotype();
		void newConnectionMutation();
		void newNeuronMutation();
		void newWeightMutation();
		void mutateActivation();
		double computeCompatibility(const Genome &g);
		void sortGenes();
		int getRandomNeuronIndex();
		double getRandomWeight();
		double Fitness();
		unsigned numGenes();
		unsigned getNInputs() { return this->nInputs; };
		unsigned getNOutputs() { return this->nOutputs; };
		void setFitness(double fitness) { this->fitness = fitness; }

	private:
		Innovation* inn;
		unsigned id;
		Net* phenotype;
		double fitness;
		double toSpawn;
		unsigned nInputs;
		unsigned nOutputs;
		unsigned specieId;
		unsigned getNeuronById(unsigned);
		bool checkDuplicate(unsigned, unsigned);
		bool neuronExists(unsigned);

		static const double mutationRate;
		unsigned newLinkTries = 5;
		unsigned newNeuronTries = 5;

	friend bool operator<(const Genome &l, const Genome &r) {
		return (l.m_dFitness > r.m_dFitness);
	}
};

#endif