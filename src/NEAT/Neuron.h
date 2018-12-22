#ifndef _PERCEPTRON_H_
#define _PERCEPTRON_H_

#include <cstdlib>
#include <cmath>
#include <vector>
#ifndef _NEURON_
#define _NEURON_
using namespace std;

class Neuron {
	public:
		vector<double> inputs;
		vector<double> weights;
		double output;
		Neuron(unsigned nInputs);

	private:
		double activationFunction(double);
		double computeOutput();
		static double randomWeightInitializer();

};

#endif