#ifndef _GENOME_
#define _GENOME_

#include "Connection.h"
#include "Node.h"

#include <vector>
#include <cstdlib>
#include <map>

using namespace std;

class Genome {

	public:
		Genome();
		Genome crossover(Genome p1, Genome p2);
		void newConnectionMutation();
		void newNodeMutation();
		void newWeightMutation();
		int getRandomNodeIndex();
		double getRandomWeight();
	
	private:
		double mutationRate = 0.9;
		map<unsigned, Connection> connections;
		map<unsigned, Node> nodes;

		
};

#endif