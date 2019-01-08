#include "Innovation.h"

unsigned Innovation::getInnovation(unsigned in, unsigned out, InnovationType inn) {
    for (size_t i = 0; i < innovations.size(); ++i)
        if (innovations[i].input == in && innovations[i].output == out && innovations[i].innovationType == inn)
            return innovations[i].innovationID;

    return -1;
}

unsigned Innovation::newInnovation(unsigned in, unsigned out, InnovationType it, Layer l = None) {
    InnovationData i(it, this->nextInnovationID, in, out, 0, l);

	if (it == neuron) {
		i.neuronID = nextNeuronID;
		++nextNeuronID;
	}

    innovations.push_back(i);
    return (++nextInnovationID);
}

Neuron Innovation::newNeuron(unsigned id) {
    Neuron n(Layer::Hidden, 0);

    for (size_t i = 0; i < innovations.size(); ++i) {
        if (innovations[i].neuronID == id) {
            n.setId(innovations[i].neuronID);
            n.setLayer(innovations[i].neuronType);

            return n;
        }
    }

    return n;
}