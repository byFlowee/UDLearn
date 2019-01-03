
#include <iostream>

#include "Perceptron.h"

using namespace std;

int main(int argc, char **argv)
{
    Perceptron p(2);

    vector<vector<double>> x;
    vector<double> aux;
    aux.push_back(0.0);
    aux.push_back(0.0);
    x.push_back(aux);
    
    aux.clear();

    aux.push_back(0.0);
    aux.push_back(1.0);
    x.push_back(aux);

    aux.clear();

    aux.push_back(1.0);
    aux.push_back(0.0);
    x.push_back(aux);

    aux.clear();

    aux.push_back(1.0);
    aux.push_back(1.0);
    x.push_back(aux);

    vector<double> targets;
    targets.push_back(-1.0);
    targets.push_back(-1.0);
    targets.push_back(-1.0);
    targets.push_back(1.0);

    p.trainPerceptron(200, x, targets);

    cout << endl;
    cout << "1 1 : " << p.getPrediction(aux) << endl;

    aux.clear();

    aux.push_back(0.0);
    aux.push_back(0.0);
    
    cout << "0 0 : " << p.getPrediction(aux) << endl;
    
    aux.clear();

    aux.push_back(0.0);
    aux.push_back(1.0);
    
    cout << "0 1 : " << p.getPrediction(aux) << endl;

    aux.clear();
  
    aux.push_back(1.0);
    aux.push_back(0.0);
    
    cout << "1 0 : " << p.getPrediction(aux) << endl;

    aux.clear();

    return 0;
}