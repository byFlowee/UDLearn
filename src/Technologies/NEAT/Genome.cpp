#include <algorithm>

#include "Genome.h"
#include "Innovation.h"

void Genome::newWeightMutation() {

}

//add connection mutation, a single new connection gene with a random weight is added connecting
//two previously unconnected neurons
void Genome::newConnectionMutation() {
	if ( ((double) rand() / RAND_MAX) > this->mutationRate ) {
		unsigned n2 = -1;
		unsigned n1 = -1;
		for (int i = 0; i < newLinkTries; ++i) {
			unsigned n2 = neurons[getRandomNeuronIndex()].getId();
			unsigned n1 = neurons[getRandomNeuronIndex()].getId();

			if (n1 == n2 || checkDuplicate(n1, n2))
				n1 = n2 = -1;
			else
				break;
		}

		//no valid pair of neurons found to make a connection
		if (n1 == -1 || n2 == -1)
			return;

		//check if this innovation already exists
		unsigned innovationId = inn->getInnovation(n1, n2, InnovationType::connection);

		if (innovationId == -1) {
			inn->newInnovation(n1, n2, InnovationType::connection, Layer::None);
			Connection c(n1, n2, getRandomWeight(), true, inn->getNextInnovationId());
			connections.push_back(c);
		} else {
			Connection c(n1, n2, getRandomWeight(), true, innovationId);
			connections.push_back(c);
		}
	}
}

void Genome::newNeuronMutation() {
	if ( ((double) rand() / RAND_MAX) > this->mutationRate ) {
		unsigned threshold = this->nInputs + this->nOutputs + 10;
		unsigned randomConnection = -1;

		//size threshold to prevent chain effect on mutations
		if (connections.size() < threshold) {
			for (size_t i = 0; i < newNeuronTries; ++i) {
				randomConnection = rand() % connections.size() - (int)sqrt((double)connections.size());	//this will get older links on the genome
				int from = connections[randomConnection].getInputId;

				if (connections[randomConnection].isEnabled() && neurons[this->getNeuronById(from)].getLayer() != Bias) {
					break;
				}
			}
		} else {
			bool found = false;
			while(!found){
				randomConnection = rand() % connections.size();
				unsigned from = connections[randomConnection].getInputId();

				if (connections[randomConnection].isEnabled() && neurons[this->getNeuronById(from)].getLayer() != Bias) {
					found = true;
				}
			}
		}

		//at this point we have a valid connection to insert the neuron into
		connections[randomConnection].disableConnection();
		double previousWeight = connections[randomConnection].getWeight();
		unsigned from = connections[randomConnection].getInputId;
		unsigned to = connections[randomConnection].getOutputId;

		unsigned innovationId = inn->getInnovation(from, to, InnovationType::neuron);

		if (innovationId != -1) {
			unsigned neuronId = inn->getNeuronID(innovationId);
			if (neuronExists(neuronId)) {
				innovationId = -1;			// new innovation is needed so id is reset to -1
			}
		}

		if (innovationId == -1) {
			unsigned newNeuron = inn->newInnovation(from, to, InnovationType::neuron, Layer::Hidden);
			neurons.push_back(Neuron(Layer::Hidden, newNeuron));

			unsigned connId = inn->getNextInnovationId();
			inn->newInnovation(from, newNeuron, InnovationType::connection, Layer::None);
			connections.push_back(Connection(from, newNeuron, 1.0, true, connId));

			connId = inn->getNextInnovationId();
			inn->newInnovation(newNeuron, to, InnovationType::connection, Layer::None);
			connections.push_back(Connection(newNeuron, to, previousWeight, true, connId));
		} else {
			//innovation already created
			unsigned newNeuron = inn->getNeuronID(innovationId);

			unsigned connId1 = inn->getInnovation(from, newNeuron, InnovationType::connection);
			unsigned connId2 = inn->getInnovation(newNeuron, to, InnovationType::connection);

			connections.push_back(Connection(from, newNeuron, 1.0, true, connId1));
			connections.push_back(Connection(newNeuron, to, previousWeight, true, connId2));

			neurons.push_back(Neuron(Layer::Hidden, newNeuron));
		}
	}
}

bool Genome::neuronExists(unsigned ID) {
	for (int n=0; n < neurons.size(); ++n)
		if (ID == neurons[n].getId())
			return true;

	return false;
}

bool Genome::checkDuplicate(unsigned in, unsigned out) {
	for (int c = 0; c < connections.size(); ++c)
		if ((connections[c].getInputId() == in) && (connections[c].getOutputId() == out))
			return true;

	return false;
}

int Genome::getRandomNeuronIndex() {
	return (rand() % neurons.size());
}

double Genome::getRandomWeight() {
	return (((double)rand() / RAND_MAX ) * 2) - 1;
}
