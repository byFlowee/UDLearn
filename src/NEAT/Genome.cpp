#include "Genome.h"

Genome::Genome() {
}

void Genome::newWeightMutation() {

	map<unsigned, Connection>::iterator it;
	for (it = connections.begin(); it != connections.end(); it++) {
		if ( ((double)rand() / RAND_MAX) < mutationRate ) {
			it->second.setWeight(it->second.getWeight() *((double)rand() / RAND_MAX)*3.0 - 1.5);
		}
		else {
			it->second.setWeight( ((double)rand() / RAND_MAX)*3.0 - 1.5);
		}
	}
}

//add connection mutation, a single new connection gene with a random weight is added connecting
//two previously unconnected nodes
void Genome::newConnectionMutation() {
	Node n1 = nodes[getRandomNodeIndex()];
	Node n2 = nodes[getRandomNodeIndex()];

	for (size_t i = 0; i < connections.size(); ++i) {
		if ( ( connections[i].getInputId() == n1.getId() && connections[i].getOutputId() == n2.getId() ) 
			|| (connections[i].getInputId() == n2.getId() && connections[i].getOutputId() == n1.getId()) ) {
			return;
		}
	}

	bool swap = false;
	if (n1.getLayer() > n2.getLayer()) {
		swap = true;
	}

	Connection n((swap ? n2.getId() : n1.getId()), (swap ? n1.getId() : n2.getId()), getRandomNodeIndex(), true, false, 0);
	connections.insert({n.getInnovation(), n});
}

void Genome::newNodeMutation() {
	Connection c = connections[getRandomNodeIndex()];

	Node input = nodes[c.getInputId()];
	Node output = nodes[c.getOutputId()];

	c.disableConnection();

	Node n(Layer(Hidden), nodes.size());
	Connection in(input.getId(), n.getId(), 1.0, true, false, 0);
	Connection out(n.getId(), output.getId(), c.getWeight(), true, false, 0);

	nodes.insert({n.getId(), n});
	connections.insert({in.getInnovation(), in});
	connections.insert({out.getInnovation(), out});
}

Genome Genome::crossover(Genome p1, Genome p2) {
	Genome child;

	//Copying all the nodes of parent 1 to the child
	for (size_t i = 0; i < p1.nodes.size(); ++i) {
		child.nodes.insert({p1.nodes[i].getId(), p1.nodes[i]});
	}
	
	//for every connection on the parent 1 genome
	map<unsigned, Connection>::iterator it;
	for (it = p1.connections.begin(); it != p1.connections.end(); it++) {
		map<unsigned, Connection>::iterator it2;
		it2 = p2.connections.find(it->first);
		
		if (it2 != p2.connections.end()) {	//match
			Connection c(rand() % 2 ? it->second : it2->second);
			child.connections.insert({c.getInnovation(), c});
		}
		else { //disjoint or excess
 			Connection c(it->second);
			child.connections.insert({c.getInnovation(), c});
		}
	}
	
	return child;
}

int Genome::getRandomNodeIndex() {
	return (rand() % nodes.size());
}

double Genome::getRandomWeight() {
	return (((double)rand() / RAND_MAX ) * 2) - 1;
}

