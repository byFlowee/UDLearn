#include "Connection.h"

Connection::Connection(unsigned in, unsigned out, double w, bool e, bool r, unsigned i) {
	this->inputNode = in;
	this->outputNode = out;
	this->weight = w;
	this->expressed = e;
	this->recurrent = r;
	this->innovation = i;
}


