#ifndef _INNOVATION_
#define _INNOVATION_

#include <vector>

#include "Neuron.h"

enum InnovationType {
    neuron,
    connection
};


struct InnovationData {
    InnovationType innovationType;
    unsigned innovationID;
    unsigned input;
    unsigned output;
    unsigned neuronID;
    Layer neuronType;

    InnovationData(InnovationType it, unsigned iid, unsigned in, unsigned out, unsigned nid, Layer nt) : innovationType(it), innovationID(iid), input(in), output(out), neuronID(nid), neuronType(nt) {};
};


class Innovation {
    private:
        std::vector<InnovationData> innovations;
        unsigned nextNeuronID;
        unsigned nextInnovationID;

    public:
        unsigned getInnovation(unsigned, unsigned, InnovationType);
        unsigned newInnovation(unsigned, unsigned, InnovationType, Layer);
        unsigned getNextInnovationId() { return this->nextInnovationID; }
        unsigned getNeuronID(unsigned innovation) const { return innovations[innovation].neuronID; }
        Neuron newNeuron(unsigned id);
};

#endif