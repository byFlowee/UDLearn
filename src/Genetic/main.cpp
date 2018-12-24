
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "GeneticNN.h"
#include "DNA.h"
#include "UtilG.h"
#include "../NeuralNetwork/neuralNetwork.h"
#include "../DataLoader/DataLoader.h"
#include "Player.h"

using namespace std;

void flattenMatrix()
{
    vector<Mat> a;

    for (int i = 0; i < 10; i++)
    {
        Mat m(i + 2, i + 1, i);

        m.set(0, 0, 99.0);
        m.set(i, 0, 22.0);
        m.print();
        cout << endl;

        a.push_back(m);
    }

    cout << endl;

    Mat flattened = UtilG::flattenMatices(a);

    cout << "Flattened matrix:" << endl;
    flattened.print();
    cout << endl;
}

void flattenManuallyNNWeightsAndBias()
{
    vector<int> topology = {2, 3, 4};

    NeuralNetwork nn(topology);

    vector<Mat> nnWeights = nn.getWeights();
    vector<Mat> nnBias = nn.getBias();

    cout << endl;
    cout << "Weights:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnWeights.size(); i++)
    {
        nnWeights[i].print();
        cout << endl << "-----------------------------" << endl;
    }

    cout << endl;
    cout << "Bias:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnBias.size(); i++)
    {
        nnBias[i].print();
        cout << endl << "-----------------------------" << endl;
    }

    Mat flattenedWeights = UtilG::flattenMatices(nnWeights);
    Mat flattenedBias = UtilG::flattenMatices(nnBias);

    cout << endl;
    cout << "Flattened weights: " << endl;
    flattenedWeights.print();

    cout << endl << endl;
    cout << "Flattened bias: " << endl;
    flattenedBias.print();

    Mat unionWeightsBias = UtilG::unionOfFlattenedMatrices(flattenedWeights, flattenedBias);

    cout << endl << endl;
    cout << "Union between flattened weights and flattened bias: " << endl;
    unionWeightsBias.print();

    vector<vector<Mat>> unflattenedWeightsAndBias = UtilG::unflattenMatrices(topology, unionWeightsBias);

    cout << endl << endl;
    cout << "Weights after unflatten:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < unflattenedWeightsAndBias[0].size(); i++)
    {
        unflattenedWeightsAndBias[0][i].print();
        cout << endl << "-----------------------------" << endl;
    }

    cout << endl;
    cout << "Bias after unflatten:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < unflattenedWeightsAndBias[1].size(); i++)
    {
        unflattenedWeightsAndBias[1][i].print();
        cout << endl << "-----------------------------" << endl;
    }
}

void flattenUtilGNNWeightsAndBias()
{
    vector<int> topology = {2, 3, 4};

    NeuralNetwork nn(topology);
    
    vector<Mat> nnWeights = nn.getWeights();
    vector<Mat> nnBias = nn.getBias();

    cout << endl;
    cout << "Weights:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnWeights.size(); i++)
    {
        nnWeights[i].print();
        cout << endl << "-----------------------------" << endl;
    }

    cout << endl;
    cout << "Bias:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnBias.size(); i++)
    {
        nnBias[i].print();
        cout << endl << "-----------------------------" << endl;
    }

    Mat representativeVector = UtilG::getARepresentativeVectorOfNeuralNetwork(nn);

    cout << endl << endl;

    representativeVector.set(0, 0, 0.1234);

    UtilG::setRepresentativeVectorOnNeuralNetwork(representativeVector, nn);

    nnWeights = nn.getWeights();
    nnBias = nn.getBias();

    cout << "Weights:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnWeights.size(); i++)
    {
        nnWeights[i].print();
        cout << endl << "-----------------------------" << endl;
    }

    cout << endl;
    cout << "Bias:" << endl << "-----------------------------" << endl;

    for (size_t i = 0; i < nnBias.size(); i++)
    {
        nnBias[i].print();
        cout << endl << "-----------------------------" << endl;
    }
}

void DNACrossoverAndMutation()
{
    Mat mA(1, 5);
    Mat mB = UtilG::getRandomMatrix(1, 5);

    DNA a(mA);
    DNA b(mB);

    cout << "DNA a:" << endl;
    a.getGenes().print();

    cout << endl;

    cout << "DNA b:" << endl;
    b.getGenes().print();

    cout << endl;

    Mat crossover1 = a.crossover(b, 0, 0).getGenes();
    Mat crossover2 = b.crossover(a, 0, 0).getGenes();
    Mat crossover3 = b.crossover(b, 0, 0).getGenes();

    cout << "Crossover a->b:" << endl;
    crossover1.print();
    cout << endl;
    cout << "Crossover b->a:" << endl;
    crossover2.print();
    cout << endl;
    cout << "Crossover b->b:" << endl;
    crossover3.print();
    cout << endl;

    cout << endl << "Mutation:" << endl;
    srand(time(NULL));

    for (int i = 0; i < 100; i++)
    {
        DNA mutated = a;

        mutated.mutate();

        if (a == mutated)
        {
            cout << "Same";
        }
        else
        {
            cout << endl;
            cout << "Different!" << endl;
            cout << "a: ";
            a.getGenes().print();
            cout << endl << "muteted: ";
            mutated.getGenes().print();
            cout << endl << endl;
        }
    }

    cout << endl;
}

NeuralNetwork play(const vector<int> &topology, Game game, int maxGenerations, int population, double mutationRate, size_t elitism, size_t weightsFactor, bool fitnessSharing)
{
    int score = 0;
    int steps = 0;
    GeneticNN geneticAlg(topology, game, population, elitism, weightsFactor);
    NeuralNetwork nn(topology);
    vector<vector<Mat>> weightsAndBias;
    DNA best;
    int bestScore = -1;
    double fitnessValue = 0.0;
    bool improvedScore = true;
    bool improvedFitness = true;

    geneticAlg.createPopulation();
    geneticAlg.setMutationRate(mutationRate);

    for (int i = 0; i < maxGenerations; i++)
	{
        
        if (fitnessSharing)
        {
            geneticAlg.fitnessSharing();
        }
        else
        {
            geneticAlg.computeFitness();
        }

        double fitnessAux = geneticAlg.getCurrentBestDNAFitness();

        if (fitnessAux > fitnessValue)
        {
            improvedFitness = true;
        }
        else
        {
            improvedFitness = false;
        }
			
        fitnessValue = fitnessAux;
        DNA currentBest = geneticAlg.getCurrentBestDNA();
        Mat genes = currentBest.getGenes();
        vector<int> playerResults;

        // Weights and bias of the best Neural Network
        weightsAndBias = UtilG::setRepresentativeVectorOnNeuralNetwork(genes, nn);

        //score = Player::playBreakout(nn);
        //playerResults = Player::playBreakout(nn, true);   // If you want to see the best DNA of the generation. Can't be closed and continue.. It has to end.

        switch (game)
        {
            case Game::breakout:
                playerResults = Player::playBreakout(nn);           // If you don't want to see the best DNA.
                break;
            case Game::boxing:
                playerResults = Player::playBoxing(nn);           // If you don't want to see the best DNA.
                break;
            case Game::demonAttack:
                playerResults = Player::playDemonAttack(nn);           // If you don't want to see the best DNA.
                break;
            case Game::starGunner:
                playerResults = Player::playStarGunner(nn);           // If you don't want to see the best DNA.
                break;
            default:
                //cerr << "ERROR: game unknown." << endl;   // Standard error output is redirected to /dev/null
                cout << "ERROR: game unknown." << endl;
        }

        if (playerResults[0] > score)
        {
            improvedScore = true;
        }
        else
        {
            improvedScore = false;
        }

        score = playerResults[0];
        steps = playerResults[1];

        //cout << "Generation " << geneticAlg.getCurrentGeneration() << ": " << best << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "------------------- Generation " << geneticAlg.getCurrentGeneration() + 1 << endl;

        if (improvedFitness)
        {
            cout << "------------------- Best fitness = \033[1;32m" << fitnessValue << "\033[0m" << endl;
        }
        else
        {
            cout << "------------------- Best fitness = \033[1;31m" << fitnessValue << "\033[0m" << endl;
        }

        if (improvedScore)
        {
            cout << "------------------- Score playing = \033[1;32m" << score << "\033[0m" << endl;
        }
        else
        {
            cout << "------------------- Score playing = \033[1;31m" << score << "\033[0m" << endl;
        }

        cout << "------------------- Steps playing = " << steps << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << "--------------------------------------------------------------------" << endl;
        cout << endl;

        if (bestScore < score)
        {
            bestScore = score;
            best = currentBest;
        }

        if (game == Game::breakout && score == 864)
        {
            cout << "MAX SCORE IN BREAKOUT: 864!" << endl;
            break;
        }

        geneticAlg.nextGeneration();
    }

    Mat flattenedWeightsAndBiasBest = best.getGenes();

    cout << "Weights and Bias flattened of the best DNA found (score = " << bestScore << "): ";
    flattenedWeightsAndBiasBest.print(", ");
    cout << endl << endl;

    UtilG::setRepresentativeVectorOnNeuralNetwork(flattenedWeightsAndBiasBest, nn);

    // See best DNA of last generation
    cout << "Best neural network is playing!" << endl;
    switch (game)
    {
        case Game::breakout:
            Player::playBreakout(nn, true);
            break;
        case Game::boxing:
            Player::playBoxing(nn, true);
            break;
        case Game::demonAttack:
            Player::playDemonAttack(nn, true);
            break;
        case Game::starGunner:
            Player::playStarGunner(nn, true);
            break;
        default:
            //cerr << "ERROR: game unknown." << endl;   // Standard error output is redirected to /dev/null
            cout << "ERROR: game unknown." << endl;
    }

    return nn;
}

int main (int argc, char **argv)
{
    if (argc != 4 && argc != 5)
    {
        cerr << "ERROR: Usage: ./main <GAME = 1 (breakout), 2 (boxing), 3 (demon attack), 4 (star gunner)> maxGenerations population [mutation]" << endl;

        return 0;
    }

    // representation SCORE GAME

    // 3190 Demon Attack
    //
    //vector<double> representation3190DemonAttack = {0.813133, -0.774577, -0.0958515, -0.177008, -0.148763, 0.385405, -0.397537, -0.685122, 0.273571, 0.077728, -0.970402, 0.558187, 0.987247, 0.0811712, 0.859851, 0.551545, 0.138882, 0.233675, 0.785428, 0.786788, 0.83853, 0.682145, -0.461455, 0.225344, 0.707928, 0.080523, 0.772754, -0.0670632, -0.389047, -0.4632, 0.797971, -0.706557, -0.0331996, 0.731363, 0.583559, -0.00152647, 0.578275, -0.671047, 0.177851, -0.659175, -0.50879, 0.140976, -0.935709, -0.977578, 0.765607, 0.968304, 0.740836, 0.754916, 0.141921, 0.662001, 0.821084, 0.108215, -0.308102, 0.786728, 0.581076, 0.799861, 0.457467, -0.61663, 0.00669438, 0.150084, 0.30884, 0.635624, -0.988787, 0.682393, 0.506785, -0.544535, -0.906322, 0.903477, 0.0197724, 0.994218, -0.644961, 0.338245, -0.521022, -0.276407, 0.118459, 0.938438, 0.188411, 0.787487, 0.833682, 0.893877, -0.136939, 0.945431, -0.146257, -0.206365, -0.985604, 0.999156, 0.342119, -0.40059, 0.570956, 0.929156, -0.422304, 0.302697, 0.589026, -0.0159715, 0.155438, -0.671096, -0.816319, 0.665957, 0.958926, -0.776088, 0.578297, 0.378642, -0.763524, -0.204632, 0.036328, -0.371469, -0.413487, 0.0527279, 0.478231, -0.206874, 0.990611, 0.865507, -0.925038, 0.470283, 0.024437, -0.529285, -0.143291, 0.252637, 0.910893, 0.851966, 0.733639, -0.284878, 0.398573, 0.966958, -0.753468, -0.860247, -0.744669, 0.00956956, 0.712016, -0.317589, 0.716868, 0.465884, -0.471777, 0.777568, 0.0986195, -0.31952, 0.177318, -0.586038, 0.827953, -0.177882, -0.761006, -0.320187, 0.730627, 0.945645, -0.337081, 0.346766, -0.596686, 0.619535, -0.214158, 0.486485, 0.947294, 0.994306, 0.140864, -0.373836, -0.818308, 0.900758, 0.717657, -0.411066, 0.948215, -0.428374, -0.348721, 0.763642, 0.257343, 0.590993, -0.15654, -0.701238, -0.55898, -0.414373, -0.286581, 0.577236, 0.557889, 0.919812, -0.912463, 0.276931, 0.230138, -0.16541, -0.670619, -0.718183, -0.729915, -0.95846, 0.152924, 0.925626, 0.915449, 0.969908, 0.472136, 0.576542, 0.681185, 0.317707, -0.465837, -0.0899306, -0.380534, 0.799333, 0.329391, 0.926844, -0.635055, 0.491557, 0.176421, 0.661799, 0.37357, -0.0616692, 0.446104, 0.484929, 0.141433, 0.351259, 0.56951, 0.0343798, 0.912932, 0.0224349, 0.778304, 0.639689, 0.0141142, -0.226282, -0.599603, 0.879284, 0.467018, -0.882895, 0.0267043, -0.385707, 0.0623254, -0.249108, -0.595004, 0.569403, -0.25369, -0.163609, 0.963592, 0.940551, -0.32936, 0.476597, 0.917219, 0.622242, 0.0298533, 0.26883, -0.739248, 0.229679, 0.591509, 0.470362, 0.980728, 0.839376, 0.729154, 0.00208779, -0.749419, 0.693697, 0.398982, -0.989627, 0.672831, 0.713558, -0.47649, -0.936477, -0.178523, 0.56229, -0.739597, -0.341913, 0.267627, 0.36464, 0.34256, -0.862573, -0.882032, 0.822427, -0.628767, -0.092459, 0.699951, -0.971049, 0.0273444, -0.882376, 0.273192, 0.796176, 0.790965, 0.298693, 0.249541, 0.788562, 0.0129524, 0.0466026, 0.778239, -0.923414, -0.8489, 0.478173, 0.618489, 0.203344, -0.364246, 0.38548, -0.739554, 0.40849, 0.917045, 0.63318, 0.5459, 0.602741, -0.240708, 0.0259421, 0.0221984, 0.38463, -0.943975, 0.275937, -0.468389, -0.890145, -0.317147, -0.284398, -0.734654, 0.840026, 0.0587229, -0.492424, 0.993937, 0.218752, 0.0150084, 0.356243, 0.774888, -0.388846, 0.324565, -0.0123203, 0.840718, -0.623888, 0.833343, -0.45765, 0.725452, 0.131956, 0.126928, 0.565363, 0.68981, 0.823393, 0.64916, -0.307702, 0.975952, 0.4124, -0.746872, 0.447231, 0.916753, -0.325485, 0.325484, 0.428285, 0.409799, 0.998879, -0.431213, 0.349894, 0.370854, 0.490039, 0.661061, -0.819021, 0.670001, 0.344118, -0.84587, 0.708872, -0.316102, 0.840828, -0.308214, 0.879197, 0.303116, 0.129515, -0.962328, 0.106951, -0.290449, 0.694407, 0.527012, 0.891579, 0.619897, 0.295789, 0.848505, -0.803352, -0.283879, 0.658266, 0.234472, -0.717275, 0.0489245, 0.560794, 0.386285, 0.359433, 0.387056, 0.701188, -0.204458, 0.152026, 0.0453204, -0.290929, -0.272418, -0.479164, -0.0628553, 0.780606, -0.861432, 0.586488, 0.740389, 0.37049, 0.770554, -0.336533, 0.358162, -0.76342, 0.725457, 0.570405, 0.727585, 0.353515, 0.43566};
    //vector<double> representation2000DemonAttack = {0.509096, 0.808852, 0.861293, 0.40755, 0.432433, 0.0247949, -0.0390703, -0.0327103, -0.266677, -0.0757034, 0.66037, -0.680567, 0.277309, -0.0327103, 0.811459, 0.38025, 0.831619, 0.721566, 0.244935, 0.833406, 0.701173, 0.470727, 0.891179, 0.794066, 0.351466, 0.631288, 0.588994, 0.845719, 0.0920311, 0.779055, 0.322858, 0.808858, 0.964636, 0.259846, 0.716632, 0.382995, 0.519284, 0.782017, -0.7088, 0.953222, -0.452707, -0.990025, 0.0347326, 0.185704,0.143754, 0.176724, -0.434045, 0.683625, 0.895635, 0.850435, 0.109918, 0.732375, -0.482866, 0.667014, -0.361589, 0.0162514, 0.185704, -0.459853, -0.483506, -0.597572, 0.570606, 0.0931398, 0.899589, -0.0559232, 0.949253, 0.0609571, 0.524518, 0.782733, 0.978077, -0.357531, 0.648264, 0.299404, 0.610075, 0.10579, -0.0930586, 0.0867941, 0.961724, 0.394886, -0.415775, 0.0743469, 0.587601, 0.23493, 0.745162, 0.311365, -0.201829, -0.752769, 0.188796, 0.938652, -0.709379, 0.389001, 0.953821, -0.752923, 0.916544, 0.369385, 0.218309, 0.104426, 0.114546, 0.629767, -0.37605, 0.944586, 0.378759, -0.384085, 0.185704, 0.724058, -0.00703857, 0.0590717, 0.0337095, 0.770104, -0.610435, -0.143146, 0.550694, 0.977166, -0.830881, 0.0342378, -0.0406486, -0.494992, -0.886115, 0.270698, 0.678571, -0.694609, 0.621457, 0.0337095, 0.800406, 0.85374, 0.553025, -0.766198, -0.127354, -0.19472, -0.584472, -0.482866, 0.0062584, 0.379084, 0.99792, -0.138805, -0.18361, 0.44134, 0.79659, 0.745748, -0.837261, -0.587009, 0.2924, 0.791815, 0.828694, -0.0655489, 0.800269, 0.910595, -0.0559232, 0.319892, 0.124732, 0.67891, -0.144815, 0.720476, -0.382253, 0.441425, 0.0652458, -0.531512, 0.579816, 0.74573, 0.615729, -0.370457, -0.452562, 0.251133, 0.181292, -0.658045, 0.382694, 0.240291, -0.234668, 0.595601, -0.644228, -0.672842, 0.365916, -0.9751, 0.548751, -0.061389, 0.261823, -0.618247, 0.562196, 0.148282, 0.989263, 0.721237, 0.887017, 0.411838, 0.601681, -0.379763, 0.294081, -0.166422, -0.761651, -0.810224, -0.357531, 0.679348, -0.17717, 0.555458, 0.153767, 0.795749, -0.368984, 0.0844785, -0.201469, 0.638196, 0.95932, 0.183127, 0.421787, -0.522736, 0.208027, -0.479987, 0.877197, 0.0146054, -0.621777, 0.506228, -0.546269, 0.672172, -0.811904, 0.722332, 0.923358, 0.187439, 0.300281, 0.214412, 0.527101, 0.366597, 0.580744, 0.682611, 0.62939, 0.997299, 0.436949, -0.644228, 0.945571, 0.498749, 0.639742, 0.280527, 0.41618, 0.3789, 0.800569, -0.459853, 0.436949, 0.0353256, 0.51005, 0.702711, 0.332207, 0.957751, 0.0791314, -0.00577779, 0.953222, 0.914746, -0.67174, -0.602293, 0.632735, -0.0327103, 0.814664, 0.968029, 0.609153, 0.717703, -0.914585, 0.577686, -0.212187, 0.749211, 0.261063, -0.42379, -0.416539, 0.935983, -0.569952, 0.963788, -0.306115, 0.850435, 0.574773, 0.518214, 0.763146, 0.539026, -0.795555, -0.810224, 0.582562, -0.0773242, 0.0206753, -0.739339, 0.144734, 0.0500573, -0.435181, 0.593871, -0.162287, -0.00795526, 0.850856, 0.4178, -0.694609, 0.914287, -0.479987, -0.569952, -0.897317, -0.463446, 0.0622835, 0.572811, 0.185824, -0.800089, 0.501563, 0.371338, 0.112879,-0.235407, 0.660572, 0.196952, -0.672842, 0.381214, 0.385389, 0.24299, -0.979731, 0.423075, 0.875508, 0.802652, -0.766198, 0.763146, 0.859297, 0.752338, -0.313855, 0.465392, 0.307347, 0.187739, 0.782172, 0.655946, 0.792053, -0.382589, 0.186827, -0.241849, 0.729608, 0.895635, 0.267123, -0.113808, 0.914746, -0.0460862, 0.454175, -0.402559, -0.27367, 0.0893502, 0.335957, 0.237278, -0.895265, 0.458046, 0.705291, -0.0517979, 0.607509, 0.872717, -9.3143e-05, -0.889389, -0.492799, 0.369385, 0.0863936, 0.804776, 0.694366, 0.615729, 0.70883, -0.379763, 0.721237, 0.333928, 0.584298, -0.500896, 0.817779, -0.266373, -0.551622, -0.162287, 0.808191, 0.5631, 0.717703, 0.670232, 0.655387, -0.479987, -0.990273, 0.988129, 0.383238, 0.497878, 0.154242, 0.946438, 0.0173179, -0.739339, 0.055238, 0.495929, 0.0577831, 0.0225522, 0.261951, 0.542821, 0.828694, 0.915582, 0.30965, -0.17717, 0.318621, -0.00795526, 0.924734, -0.0244084, -0.0203223, 0.713861, 0.607509, -0.987574, 0.77248};
    //vector<double> representation1700DemonAttack = {-0.482924, -0.187723, 0.948261, 0.518644, 0.0371773, -0.140674,-0.383597, 0.734286, -0.896439, 0.386089, -0.104974, 0.157505, 0.591177, -0.892417, -0.750745, 0.0469943, 0.0833043, 0.85466, -0.503477, 0.245889, 0.156223, -0.325726, -0.248046, -0.30469, -0.475242, -0.0937668, 0.214768, -0.365601, -0.108568, 0.950865, -0.808394, 0.408507, -0.236858, -0.860134, -0.0728482, 0.80032, -0.000808278, 0.543555, 0.534606, 0.102753, -0.0703554, -0.570368, -0.739742, -0.479178, -0.462785, -0.490487, 0.567816, 0.620519, -0.635827, -0.935661, -0.133592, 0.520396, -0.261387, 0.618362, -0.784293, 0.263371, -0.475405, 0.430474, 0.89777, 0.416027, 0.381339, -0.910624, -0.175465, -0.855518, -0.770758, 0.751686, 0.944801, 0.228434, 0.295242, 0.479407, -0.668814, -0.775114, 0.909039, -0.408556, -0.254292, -0.553746, 0.100958, -0.686476, -0.933227, 0.465131, -0.622136, -0.0668186, -0.0144726, 0.116477, -0.448456, 0.201234, -0.620152, 0.076139, -0.368292, -0.722382, -0.507834, -0.986952, -0.633006, 0.316701, -0.842471, -0.403764, 0.068387, -0.897669, 0.824669, -0.636371, 0.581738, -0.844144, -0.411485, 0.490776, -0.2527, 0.334223, 0.93703, 0.848258, 0.647747, -0.996196, 0.313389, -0.974389, -0.0630147, -0.701084, 0.142088, 0.488529, 0.50015, 0.521936, -0.435332, -0.868142, 0.799555, 0.056834, -0.855094, -0.833452, -0.626465, -0.697564, -0.237216, 0.441922, -0.595234, -0.412546, 0.80555, 0.986504, -0.256691, -0.605935, 0.47728, 0.490609, 0.728288, 0.414311, 0.338867, 0.376035, 0.418114, -0.347745, 0.401646, -0.6449, -0.0488285, -0.456266, 0.843629, -0.548678, -0.934329, -0.591703, -0.41682, 0.865225, 0.465131, -0.271914, -0.968226, 0.838665, 0.0305215, -0.205442, 0.280587, 0.435288, 0.382011, 0.0861371, 0.421792, -0.874679, 0.480202, -0.100928, 0.61593, 0.20849, -0.686618, -0.0452034, -0.415474, 0.731497, 0.607052, 0.986172, -0.913404, -0.441776, -0.470094, 0.930225, 0.00954513, -0.404423, -0.661478, 0.592725, -0.539198, 0.803652, -0.679189, -0.507424, 0.642317, 0.351333, 0.287134, -0.0770957, -0.21338, -0.330854, -0.990959, -0.791588, -0.205534, 0.489244, 0.107484, -0.589604, -0.302266, 0.420866, 0.365193, 0.282259, 0.152363, -0.0277551, 0.268431, 0.238959, 0.530469, 0.798338, 0.169184, -0.459986, -0.606085, 0.507706, -0.867261, -0.145283,0.311358, -0.54645, 0.347294, -0.0463244, 0.804882, -0.365572, 0.87658, -0.408497, 0.303573, 0.885621, -0.200085, -0.90196, 0.374865, 0.907398, -0.491564, -0.927401, 0.328264, 0.873629, 0.354858, -0.519373, -0.154126, -0.37671, 0.719586, -0.623658, -0.578373, -0.11123, -0.0836441, -0.184458, -0.603524, 0.0490946, 0.67026, 0.707834, 0.502644, 0.0175532, -0.338491, 0.307527, 0.651981, -0.461911, 0.899029, -0.0444456, -0.576289, -0.301056, 0.0535942, 0.798576, -0.393658, 0.56203, 0.871174, 0.934606, 0.435659, 0.226033, -0.584767, -0.718467, 0.849322, -0.865182, -0.342125, -0.72905, 0.0235882, 0.574231, 0.0864918, 0.420064, -0.376675, -0.243249, 0.127898, -0.87403, 0.774305, 0.789407, 0.433497, 0.426286, -0.672503, 0.332526, -0.61816, -0.248793, -0.96853, 0.435434, -0.450217, -0.362188, -0.00253568, -0.579043, -0.427581, -0.566877, 0.64699, -0.0123487, -0.285344, 0.496312, 0.12247, 0.372531, 0.767262, -0.853942, -0.0532381, -0.146247, 0.566122, 0.570087, 0.610505, -0.305981, 0.696057, 0.384809, -0.516573, 0.129554, -0.188905, -0.189077, -0.53792, 0.192935, 0.562131, -0.50645, -0.371631, -0.888087, 0.131362, 0.625834, -0.467129, 0.703781, -0.941043, -0.820139, -0.308568, -0.226387, 0.676173, 0.813902, -0.853856, 0.443434, 0.95996, 0.0929056, -0.702812, 0.526082, -0.337007, 0.907693, -0.779899, -0.64095, 0.292502, -0.296472, 0.488604, -0.896403, 0.514451, 0.950684, 0.296532, 0.0765815, -0.555766, 0.924901, 0.188495, 0.575596, 0.550735, 0.721366, 0.279377, 0.609692, 0.901226, 0.97081, -0.616696, 0.577399, 0.784712, -0.470552, 0.0208335, 0.744671, 0.622354, 0.318021, 0.270753, -0.714653, 0.225714, 0.490854, -0.355603, -0.481784, -0.805618, -0.866999, -0.378187, 0.708832, -0.916315, 0.918345, -0.214586, -0.472081, 0.843246, 0.973909, -0.896485, 0.393981, 0.695275, 0.382892, 0.00367264};
    //Mat representation3190DemonAttackMat = UtilG::getMatFromVector(representation3190DemonAttack);
    //vector<int> topology3190DemonAttack = {128, 3};
    //NeuralNetwork nn3190DemonAttack(topology3190DemonAttack);
    //UtilG::setRepresentativeVectorOnNeuralNetwork(representation3190DemonAttackMat, nn3190DemonAttack);
    //Player::playDemonAttack(nn3190DemonAttack, true);

    // 864 Breakout
    //
    //vector<double> representation864Breakout = {0.910057, 0.967478, -0.356777, 0.0107037, -0.916711, 0.0107037, -0.9124, 0.95785, 0.696089, 0.941645};
    //Mat representation864BreakoutMat = UtilG::getMatFromVector(representation864Breakout);
    //vector<int> topology864Breakout = {4, 2};
    //NeuralNetwork nn864Breakout(topology864Breakout);
    //UtilG::setRepresentativeVectorOnNeuralNetwork(representation864BreakoutMat, nn864Breakout);
    //Player::playBreakout(nn864Breakout, true);

    vector<int> topologyBreakout = {4, 2};
    vector<int> topologyBoxing = {2, 5};
    vector<int> topologyDemonAttack = {11, 3};
    vector<int> topologyStarGunner = {12, 5};

    // Redirect error output to /dev/null -> all messages in ALE are displayed in the error output.......
    int save_out = dup(STDERR_FILENO);
    int devNull = open("/dev/null", O_WRONLY);
    bool error = false;

    dup2(devNull, STDERR_FILENO);

    double mutationRate = 0.1;
    size_t elitism = 1;
    size_t weightsFactor = 1;

    if (argc == 5)
    {
        mutationRate = atof(argv[4]);
    }

    Game game = (Game)atoi(argv[1]);

    switch (game)
    {
        case Game::breakout:
            //play(topologyBreakout, game, atoi(argv[2]), atoi(argv[3]), mutationRate, elitism, weightsFactor, false);
            play(topologyBreakout, game, atoi(argv[2]), atoi(argv[3]), mutationRate, 1, 1, false); // Max. score with this configuration in ~5 generations with a population of 200.
            break;
        case Game::boxing:
            play(topologyBoxing, game, atoi(argv[2]), atoi(argv[3]), mutationRate, elitism, weightsFactor, false);
            break;
        case Game::demonAttack:
            play(topologyDemonAttack, game, atoi(argv[2]), atoi(argv[3]), mutationRate, elitism, weightsFactor, true);
            break;
        case Game::starGunner:
            play(topologyStarGunner, game, atoi(argv[2]), atoi(argv[3]), mutationRate, elitism, weightsFactor, false);
            break;
        default:
            error = true;
    }

    // Restore error output
    dup2(save_out, STDERR_FILENO);

    if (error)
    {
        cerr << "ERROR: unknown game." << endl;
    }

    return 0;
}