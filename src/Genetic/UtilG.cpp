
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

/**
 * If A =
 * [ 1 2 3 ]
 * [ 4 5 6 ]
 *
 * and B =
 * [ 7 8 9 ]
 * 
 * callThisFunction(A, B) =
 * [ 1 2 3 ]
 * [ 4 5 6 ]
 * [ 7 8 9 ]
 * 
 * A.cols() == B.cols()
 *
 * If A.cols() != B.cols(), it returns an empty matrix.
 */
Mat UtilG::unionMatrix(const Mat& a, const Mat& b)
{
    Mat res(a.rows() + b.rows(), a.cols());

    if (a.cols() == b.cols())
    {
        for (int row = 0; row < a.rows(); row++)
        {
            for (int col = 0; col < a.cols(); col++)
            {
                res.set(row, col, a.get(row, col));
            }
        }

        for (int row = a.rows() + 1; row < a.rows() + b.rows(); row++)
        {
            for (int col = 0; col < b.cols(); col++)
            {
                res.set(row, col, a.get(row, col));
            }
        }
    }

    return res;
}

vector<vector<Mat>> unflattenMatrices(const vector<int> &topology, const Mat &flattened)
{
    vector<vector<Mat>> res;
    vector<Mat> weights;
    vector<Mat> bias;
    int flattenedRow = 0;

    for (size_t i = 0; i < topology.size() - 1; i++)
    {
        //Mat matWeights(this->size[i], this->size[i + 1]);
        //Mat matBias(1, this->size[i + 1]);
        Mat matWeights(topology[i], topology[i + 1]);

        for (int row = 0; row < matWeights.rows(); row++)
        {
            for (int col = 0; col < matWeights.cols(); col++)
            {
                matWeights.set(row, col, flattened.get(flattenedRow, 0));

                flattenedRow++;
            }

            weights.push_back(matWeights);
        }
    }

    for (size_t i = 0; i < topology.size() - 1; i++)
    {
        Mat matBias(1, topology[i + 1]);

        for (int row = 0; row < matBias.rows(); row++)
        {
            matBias.set(row, 0, flattened.get(flattenedRow, 0));

            flattenedRow++;
        }

        weights.push_back(matBias);
    }

    res.push_back(weights);
    res.push_back(bias);

    return res;
}
