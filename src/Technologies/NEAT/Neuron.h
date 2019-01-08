#ifndef _NEURON_
#define _NEURON_

enum Layer {
	Input,
	Hidden,
	Bias,
	Output,
	None
};

class Neuron {
	private:
		unsigned id;
		Layer type;
		double activation;

	public:
		Neuron(Layer t, unsigned i) : type(t), id(i) {};
		unsigned getId() { return this->id; }
		Layer getLayer() { return this->type; }
		unsigned setId(unsigned id) { this->id = id; }
		Layer setLayer(Layer l) { this->type = l; }
};

#endif