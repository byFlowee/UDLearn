#ifndef _CONNECTION_
#define _CONNECTION_

class Connection {
	private:
		unsigned inputNode;
		unsigned outputNode;
		double weight;
		bool expressed;
		bool recurrent;
		unsigned innovation;
		unsigned static nextInnovation;

	public:
		Connection(unsigned, unsigned, double, bool, bool, unsigned);
		unsigned getInputId() { return this->inputNode; }
		unsigned getOutputId() { return this->outputNode; }
		unsigned getInnovation() { return this->innovation; }
		unsigned static getNextInnovation() { return nextInnovation++; }
		double getWeight() { return this->weight; }
		void setWeight(double weight) { this->weight = weight; }
		void disableConnection() { this->expressed = false;};
		
};

#endif _CONNECTION_

