
#include "ADN.h"
#include "UtilG.h"

ADN::ADN() :
    mutation(0.01),
    genes(Mat(1, 1, 1.0))
{
    // Default genes: [1.0]
}

ADN::ADN(const Mat& m, double mutation) :
    mutation(mutation),
    genes(m)
{

}

void ADN::setMutation(double mutation)
{
    this->mutation = mutation;
}

ADN ADN::crossover(const ADN& a)
{
    ADN res(a.genes, this->mutation);

    for (int row = 0; row < this->genes.rows(); row++)
    {
        for (int col = 0; col < this->genes.cols(); col++)
        {
            if (row + col % 2 == 0)
            {
                res.genes.set(row, col, this->genes.get(row, col));
            }
        }
    }

    return res;
}

void ADN::mutate()
{
    for (int row = 0; row < this->genes.rows(); row++)
    {
        for (int col = 0; col < this->genes.cols(); col++)
        {
            double random = UtilG::getRandomDouble(0.0, 1.0);

            if (random <= this->mutation)
			{
				this->genes.set(row, col, UtilG::getRandomDouble(0.0, 1.0));
			}
        }
    }
}