
#include "UtilG.h"

Mat UtilG::getRandomMatrix(size_t rows, size_t cols)
{   
    Mat a(rows, cols);

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            a.set(r, c, UtilG::getRandomDouble(-1.0, 1.0));                        
        }    
    }

    return a;
}

Mat UtilG::flattenMatices(const vector<Mat>& v)
{
    int size = 0;

    for (size_t i = 0; i < v.size(); i++)
    {
        size += v[i].size();
    }
    
    Mat res(1, size);
    int index = 0;

    for (size_t i = 0; i < v.size(); i++)
    {
        for (int row = 0; row < v[i].rows(); row++)
        {
            for (int col = 0; col < v[i].cols(); col++)
            {
                res.set(0, index, v[i].get(row, col));
                index++;
            }
        }
    }

    return res;
}

Mat UtilG::unionOfFlattenedMatrices(const Mat& a, const Mat& b)
{
    Mat res(1, a.cols() + b.cols());

    if (a.rows() == 1 && b.rows() == 1)
    {
        int col = 0;

        for (int i = 0; i < a.cols(); i++)
        {
            res.set(0, col, a.get(0, i));
            col++;
        }

        for (int i = 0; i < b.cols(); i++)
        {
            res.set(0, col, b.get(0, i));
            col++;
        }
    }

    return res;
}

vector<vector<Mat>> UtilG::unflattenMatrices(const vector<int> &topology, const Mat &flattened)
{
    vector<vector<Mat>> res;
    vector<Mat> weights;
    vector<Mat> bias;
    int flattenedCol = 0;

    for (size_t i = 0; i < topology.size() - 1; i++)
    {
        //Mat matWeights(this->size[i], this->size[i + 1]);
        //Mat matBias(1, this->size[i + 1]);
        Mat matWeights(topology[i], topology[i + 1]);

        for (int row = 0; row < matWeights.rows(); row++)
        {
            for (int col = 0; col < matWeights.cols(); col++)
            {
                matWeights.set(row, col, flattened.get(0, flattenedCol));

                flattenedCol++;
            }
        }

        weights.push_back(matWeights);
    }

    for (size_t i = 0; i < topology.size() - 1; i++)
    {
        Mat matBias(1, topology[i + 1]);

        for (int col = 0; col < matBias.cols(); col++)
        {
            matBias.set(0, col, flattened.get(0, flattenedCol));

            flattenedCol++;
        }

        bias.push_back(matBias);
    }

    res.push_back(weights);
    res.push_back(bias);

    return res;
}

Mat UtilG::getARepresentativeVectorOfNeuralNetwork(const NeuralNetwork &nn)
{
    vector<Mat> weights = nn.getWeights();
    vector<Mat> bias = nn.getBias();

    Mat flattenedWeights = UtilG::flattenMatices(weights);
    Mat flattenedBias = UtilG::flattenMatices(bias);

    Mat unionWeightsBias = UtilG::unionOfFlattenedMatrices(flattenedWeights, flattenedBias);

    return unionWeightsBias;
}

Mat UtilG::getRandomRepresentativeVectorOfNeuralNetwork(const vector<int> &topology)
{
    NeuralNetwork nn(topology);

    return UtilG::getARepresentativeVectorOfNeuralNetwork(nn);
}

vector<vector<Mat>> UtilG::setRepresentativeVectorOnNeuralNetwork(const Mat &unionWeightsBias, NeuralNetwork &nn)
{
    vector<int> topology = nn.getTopology();

    vector<vector<Mat>> unflattenedWeightsAndBias = UtilG::unflattenMatrices(topology, unionWeightsBias);

    nn.setWeights(unflattenedWeightsAndBias[0]);
    nn.setBias(unflattenedWeightsAndBias[1]);

    return unflattenedWeightsAndBias;
}