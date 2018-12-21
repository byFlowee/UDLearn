#include "Connection.h"

Connection::Connection(unsigned in, unsigned out, double w, bool e, unsigned i) {
	this->inputNode = in;
	this->outputNode = out;
	this->weight = w;
	this->expressed = e;
	this->innovation = i;
}

void Connection::disableConnection() {
	
}


