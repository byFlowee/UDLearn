
#include "DNA.h"
#include "UtilG.h"

bool operator==(const DNA &d1, const DNA &d2)
{
    return (UtilG::compareDouble(d1.mutationRate, d2.mutationRate) && 
            UtilG::compareDouble(d1.crossoverRate, d2.crossoverRate) && 
            UtilG::compareDouble(d1.genes, d2.genes));
}

DNA& DNA::operator=(const DNA& dna)
{
    this->mutationRate = dna.mutationRate;
    this->crossoverRate = dna.crossoverRate;
    this->genes = dna.genes;

    return *this;
}

DNA::DNA(int rows, int cols) :
    mutationRate(0.01),
    crossoverRate(0.7),
    genes(Mat(rows, cols, 1.0))
{
    // Default genes: [1.0]
}

DNA::DNA(const Mat& m, double mutationRate, double crossoverRate) :
    mutationRate(mutationRate),
    crossoverRate(crossoverRate),
    genes(m)
{

}

void DNA::setMutationRate(double mutationRate)
{
    this->mutationRate = mutationRate;
}

void DNA::setCrossoverRate(double crossoverRate)
{
    this->crossoverRate = crossoverRate;
}

/**
 * Take care, this->mutationRate is the mutation
 *  that is going to get the crossover, not
 *  a.mutation
 */
DNA DNA::crossover(const DNA &a, int ownFitness, int aFitness)
{
    //DNA res(a.genes, this->mutationRate);
    DNA res(a.genes.rows(), a.genes.cols());

    double random = UtilG::getRandomDouble(0.0, 1.0);

    if (random <= this->crossoverRate)
    {
        res.setMutationRate(this->mutationRate);
        res.setCrossoverRate(this->crossoverRate);

        if (this->genes.rows() == a.genes.rows() &&
            this->genes.cols() == a.genes.cols())
        {
            for (int row = 0; row < this->genes.rows(); row++)
            {
                for (int col = 0; col < this->genes.cols(); col++)
                {
                    /*
                    if (row + col % 2 == 0)
                    {
                        res.genes.set(row, col, this->genes.get(row, col));
                    }
                    */

                // Average
                res.genes.set(row, col, (this->genes.get(row, col) + a.genes.get(row, col)) / 2.0);
                }
            }
        }
    }
    else
    {
        if (ownFitness > aFitness)
        {
            res = *this;
        }
        else
        {
            res = a;
        }
    }

    return res;
}

void DNA::mutate(size_t factor)
{
    for (int row = 0; row < this->genes.rows(); row++)
    {
        for (int col = 0; col < this->genes.cols(); col++)
        {
            double random = UtilG::getRandomDouble(0.0, 1.0);

            if (random <= this->mutationRate)
			{
				this->genes.set(row, col, UtilG::getRandomDouble((double)factor * -1.0, (double)factor * 1.0));
			}
        }
    }
}

void DNA::mutatePermutation()
{
    for (int row = 0; row < this->genes.rows(); row++)
    {
        for (int col = 0; col < this->genes.cols(); col++)
        {
            double random = UtilG::getRandomDouble(0.0, 1.0);

            if (random <= this->mutationRate)
			{
                int randomRow1 = rand() % this->genes.rows();
                int randomCol1 = rand() % this->genes.cols();
                int randomRow2 = rand() % this->genes.rows();
                int randomCol2 = rand() % this->genes.cols();
                double value1 = this->genes.get(randomRow1, randomCol1);
                double value2 = this->genes.get(randomRow2, randomCol2);

                this->genes.set(randomRow1, randomCol1, value2);
                this->genes.set(randomRow2, randomCol2, value1);
			}
        }
    }
}

Mat DNA::getGenes() const
{
    return this->genes;
}