#include "Neat.h"
#include <algorithm>

Genome Neat::crossover(Genome &p1, Genome &p2) {
	bool p1best = false;
	if (p1.Fitness() == p2.Fitness()) {
		if (p1.numGenes() == p1.numGenes()) {
			p1best = rand() % 2;
		}
		else {
			if (p1.numGenes() > p2.numGenes()) {
				p1best = true;
			}
		}
	}
	else {
		if (p1.Fitness() > p2.Fitness()) {
			p1best = true;
		}
	}

	vector<unsigned> tmp;
	vector<Neuron> childNeurons;
	vector<Connection> childConnections;

	vector<Connection>::iterator itp1 = p1.connections.begin();
	vector<Connection>::iterator itp2 = p2.connections.begin();

	Connection c;

	while ( !((itp1 == p1.connections.end()) && (itp2 == p2.connections.end())) ) {
		// p1 ended but p2 not
		if (itp1 == p1.connections.end() && itp2 != p2.connections.end()) {
			if (!p1best) {
				c = *itp2;
			}

			itp2++;
		}
		// p2 ended but p1 not
		else if (itp1 != p1.connections.end() && itp2 == p2.connections.end()) {
			if (p1best) {
				c = *itp1;
			}

			itp1++;
		}
		//disjoint check
		else if (itp1->getInnovation() < itp2->getInnovation()) {
			if (p1best)
				c = *itp1;

			itp1++;
		}
		else if (itp1->getInnovation() > itp2->getInnovation()) {
			if (!p1best)
				c = *itp2;

			itp2;
		}
		else {
			if (rand() % 2)
				c = *itp1;
			else
				c = *itp2;

            itp1++;
            itp2++;
		}

		if (childConnections.back().getInnovation() != c.getInnovation())
			childConnections.push_back(c);


        if (!alreadyAdded(c.getInputId(), tmp)) tmp.push_back(c.getInputId());
        if (!alreadyAdded(c.getOutputId(), tmp)) tmp.push_back(c.getOutputId());
	}

	sort(tmp.begin(), tmp.end());

	for (size_t i = 0; i < tmp.size(); ++i)
		childNeurons.push_back(inn->newNeuron(tmp[i]));

    return Genome(nextGenomeId++, childNeurons, childConnections, p1.getNInputs(), p1.getNOutputs());
}

bool Neat::alreadyAdded(unsigned id, vector<unsigned> &v) {
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i] == id)
            return true;

    return false;
}

vector<Net*> Neat::Epoch(const vector<double> &fitnessScores) {
    //Kill poor performing phenotypes and species
    
}