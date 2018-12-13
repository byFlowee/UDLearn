
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