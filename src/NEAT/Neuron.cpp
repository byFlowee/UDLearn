#include "Neuron.h"
#include <cstdlib>
#include <cmath>

Neuron::Neuron(unsigned nInputs) {
	for (size_t i = 0; i < nInputs; ++i) {
		weights.push_back(this->randomWeightInitializer());
	}
}

double Neuron::activationFunction(double x) {
	return 1 / (1 + (exp((double)-this->output)));
}

double Neuron::computeOutput() {
	double sum = 0;
	for (size_t i = 0; i < this->inputs.size(); ++i)
		sum += this->inputs[i] * this->weights[i];

	return this->activationFunction(sum);
}

double Neuron::randomWeightInitializer() {
	return (double)rand() / double(RAND_MAX);
}