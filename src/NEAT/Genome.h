#ifndef _GENOME_
#define _GENOME_

#include <vector>
#include "Connection.h"
#include "Node.h"

using namespace std;

class Genome {

	public:
		Genome();
		void addConnection();
		void addNode();
		int getRandomNodeIndex();
		double getRandomWeight();
	
	private:
		vector<Connection> connections;
		vector<Node> nodes;

		
};

#endif