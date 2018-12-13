
#include "UtilG.h"

Mat UtilG::getRandomMatrix(unsigned rows, unsigned cols)
{   
    Mat a(rows, cols);

    for (unsigned r = 0; r < rows; ++r) {
        for (unsigned c = 0; c < cols; ++c) {
            a.set(r, c, UtilG::getRandomDouble(-1.0, 1.0));                        
        }    
    }

    return a;
}

/**
 * We expect to get all column matrices.
 */
Mat UtilG::flattenMatrix(const vector<Mat>& v)
{
    int size = 0;

    for (unsigned i = 0; i < v.size(); i++)
    {
        size += v[i].size();
    }
    
    Mat res(size, 1);

    for (unsigned i = 0; i < v.size(); i++)
    {
        for (int row = 0; row < v[i].rows(); row++)
        {
            for (int col = 0; col < v[i].cols(); col++)
            {
                // TODO check if correct:
                res.set((i + 1) * (row + 1) * (col + 1) - 1, 1, v[i].get(row, col));
            }
        }
    }

    return res;
}