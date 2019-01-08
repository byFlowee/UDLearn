#ifndef _CONNECTION_
#define _CONNECTION_

class Connection {
	private:
		unsigned inputNeuron;
		unsigned outputNeuron;
		double weight;
		bool expressed;
		unsigned innovation;
		unsigned static nextInnovation;

	public:
		Connection();
		Connection(unsigned input, unsigned output, double weight, bool expressed, unsigned innovation);
		unsigned getInputId() { return this->inputNeuron; }
		unsigned getOutputId() { return this->outputNeuron; }
		unsigned getInnovation() { return this->innovation; }
		unsigned static getNextInnovation() { return nextInnovation++; }
		double getWeight() { return this->weight; }
		void setWeight(double weight) { this->weight = weight; }
		bool isEnabled() { return this->expressed; }
		void disableConnection() { this->expressed = false; };

};

#endif _CONNECTION_