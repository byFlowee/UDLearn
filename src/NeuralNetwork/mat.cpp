
#include <iostream>

#include "mat.h"

Mat& Mat::operator=(const Mat& mat)
{
	this->mat = mat.mat;

	return *this;
}

/*
void Mat::newMat(int rows, int cols, double defaultValue)
{
	//this.mat = new ArrayList<ArrayList<Double> >();
	
	for (int i = 0; i < rows; i++)
	{
		//this.mat.add(new ArrayList<>());
		
		for (int j = 0; j < cols; j++)
		{
			//this.mat.get(i).add(defaultValue);
			//this->mat[i].push_back(defaultValue);
			this->mat[i][j] = defaultValue;
		}
	}
}
*/

Mat::Mat(int rows, int cols, double defaultValue) :
	mat(rows, vector<double>(cols, defaultValue))
{
	
}

Mat::Mat(int rows, int cols) :
	mat(rows, vector<double>(cols, 1.0))
{
	
}

void Mat::set(int row, int col, double value)
{
	this->mat[row][col] = value;
}

double Mat::get(int row, int col) const
{
	return this->mat[row][col];
}

int Mat::rows() const
{
	return this->mat.size();
}

int Mat::cols() const
{
	if (this->mat.size() == 0)
	{
		return 0;
	}

	return this->mat[0].size();
}

int Mat::size() const
{
	return this->rows() * this->cols();
}

void Mat::scalar(double scalar)
{
	for (int i = 0; i < this->rows(); i++)
	{
		for (int j = 0; j < this->cols(); j++)
		{
			this->set(i, j, this->get(i, j) * scalar);
		}
	}
}

void Mat::transpose()
{
	int rows = this->rows();
	int cols = this->cols();
	//vector<vector<double> > matAux(cols, vector<double>(rows));
	Mat matAux(cols, rows);
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			//matAux[j][i] = this->mat[i][j];
			matAux.set(j, i, this->get(i, j));
		}
	}
	
	//this->mat = matAux;
	*this = matAux;
}

Mat Mat::mult(const Mat &b) const
{
	Mat res(this->rows(), b.cols());

	if (this->cols() == b.rows())
	{
		for (int i = 0; i < this->rows(); i++)
		{
			for (int j = 0; j < b.cols(); j++)
			{
				double value = 0.0;
				
				for (int k = 0; k < this->cols(); k++)
				{
					value += this->get(i, k) * b.get(k, j);
				}
				
				res.set(i, j, value);
			}
		}
	}
	else
	{
		cerr << "ERROR: cannot multiply that matrices. Returning default matrix." << endl;
	}
	
	return res;
}

Mat Mat::directMult(const Mat &b) const
{
	Mat res(this->rows(), this->cols());
	
	if (this->rows() == b.rows() && this->cols() == b.cols())
	{
		for (int i = 0; i < this->rows(); i++)
		{
			for (int j = 0; j < this->cols(); j++)
			{
				res.set(i, j, this->get(i, j) * b.get(i, j));
			}
		}
	}
	else
	{
		cerr << "ERROR: cannot calculate direct multiplication of that matrices. Returning default matrix." << endl;
	}
	
	return res;
}

Mat Mat::sum(const Mat &b) const
{
	Mat res(this->rows(), this->cols());
	
	if (this->rows() == b.rows() && this->cols() == b.cols())
	{
		for (int i = 0; i < this->rows(); i++)
		{
			for (int j = 0; j < this->cols(); j++)
			{
				res.set(i, j, this->get(i, j) + b.get(i, j));
			}
		}
	}
	else
	{
		cerr << "ERROR: cannot sum that matrices. Returning default matrix." << endl;
	}
	
	return res;
}

Mat Mat::sub(const Mat &b) const
{
	Mat res(this->rows(), this->cols());
	
	if (this->rows() == b.rows() && this->cols() == b.cols())
	{
		for (int i = 0; i < this->rows(); i++)
		{
			for (int j = 0; j < this->cols(); j++)
			{
				res.set(i, j, this->get(i, j) - b.get(i, j));
			}
		}
	}
	else
	{
		cerr << "ERROR: cannot subtract that matrices. Returning default matrix." << endl;
	}
	
	return res;
}

Mat Mat::copy() const
{
	Mat copy(this->rows(), this->cols());
	
	for (int i = 0; i < this->rows(); i++)
	{
		for (int j = 0; j < this->cols(); j++)
		{
			copy.set(i, j, this->get(i, j));
		}
	}
	
	return copy;
}

void Mat::print(string delimiter) const
{
	for (int i = 0; i < this->rows(); i++)
	{
		for (int j = 0; j < this->cols(); j++)
		{
			cout << this->mat[i][j];

			if (j + 1 != this->cols())
			{
				cout << delimiter;
			}
		}

		if (i + 1 != this->rows())
		{
			cout << endl;
		}
	}
}

void Mat::print(ofstream &file, string delimiter) const
{
	for (int i = 0; i < this->rows(); i++)
	{
		for (int j = 0; j < this->cols(); j++)
		{
			file << this->mat[i][j];

			if (j + 1 != this->cols())
			{
				file << delimiter;
			}
		}

		if (i + 1 != this->rows())
		{
			file << endl;
		}
	}
}