#include "Connection.h"

Connection::Connection(unsigned in, unsigned out, double w, bool e, unsigned i) {
	this->inputNeuron = in;
	this->outputNeuron = out;
	this->weight = w;
	this->expressed = e;
	this->innovation = i;
}
