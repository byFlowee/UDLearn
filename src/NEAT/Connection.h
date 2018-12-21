#ifndef _CONNECTION_
#define _CONNECTION_

class Connection {
	private:
		unsigned inputNode;
		unsigned outputNode;
		double weight;
		bool expressed;
		unsigned innovation;

	public:
		Connection(unsigned, unsigned, double, bool, unsigned);
		unsigned getInputId() { return this->inputNode; }
		unsigned getOutputId() { return this->outputNode; }
		double getWeight() { return this->weight; }
		void disableConnection();

};

#endif _CONNECTION_

