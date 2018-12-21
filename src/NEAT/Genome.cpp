#include "Genome.h"

Genome::Genome() {

}

//add connection mutation, a single new connection gene with a random weight is added connecting
//two previously unconnected nodes
void Genome::addConnection() {
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

	Connection n((swap ? n2.getId() : n1.getId()), (swap ? n1.getId() : n2.getId()), getRandomNodeIndex(), true, 0);
	connections.push_back(n);
}

void Genome::addNode() {
	Connection c = connections[getRandomNodeIndex()];

	Node input = nodes[c.getInputId()];
	Node output = nodes[c.getOutputId()];

	c.disableConnection();

	Node n(Layer(Hidden), nodes.size());
	Connection in(input.getId(), n.getId(), 1.0, true, 0)
}

int Genome::getRandomNodeIndex() {
	return (rand() % nodes.size());
}

double Genome::getRandomWeight() {
	return (((double)rand() / RAND_MAX ) * 2) - 1;
}

