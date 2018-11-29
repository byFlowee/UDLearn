#include "Network.h"
#include <iostream>

int main() {
    vector<unsigned> topology;
    topology.push_back(8);
    topology.push_back(4);
    topology.push_back(2);
    topology.push_back(2);
    
    Network n(topology);

    n.printTopology();

    return 0;
}