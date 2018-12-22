#ifndef _NODE_
#define _NODE_ 

enum Layer {
	Input,
	Hidden,
	Output
};	

class Node {
	private:
		unsigned id;
		Layer type;

	public:
		Node(Layer, unsigned);
		unsigned getId() { return this->id; }
		Layer getLayer() { return this->type; }
};

#endif