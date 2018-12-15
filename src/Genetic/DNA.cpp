
#include "DNA.h"
#include "UtilG.h"

bool operator==(const DNA &d1, const DNA &d2)
{
    return (UtilG::compareDouble(d1.mutation, d2.mutation) && 
            UtilG::compareDouble(d1.genes, d2.genes));
}

DNA::DNA() :
    mutation(0.01),
    genes(Mat(1, 1, 1.0))
{
    // Default genes: [1.0]
}

DNA::DNA(const Mat& m, double mutation) :
    mutation(mutation),
    genes(m)
{

}

void DNA::setMutation(double mutation)
{
    this->mutation = mutation;
}

/**
 * Take care, this->mutation is the mutation
 *  that is going to get the crossover, not
 *  a.mutation
 */
DNA DNA::crossover(const DNA &a)
{
    DNA res(a.genes, this->mutation);

    if (this->genes.rows() == a.genes.rows() &&
        this->genes.cols() == a.genes.cols())
    {
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
    }

    return res;
}

void DNA::mutate()
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

Mat DNA::getGenes() const
{
    return this->genes;
}