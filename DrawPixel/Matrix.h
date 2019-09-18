#pragma once

#include <vector>
#include "Vector2.h"

class Matrix
{
private:
	std::vector<std::vector<float>> mat;
	unsigned rows;
	unsigned cols;
public:
	Matrix(unsigned rows, unsigned cols);	
	virtual ~Matrix();

	/*float getRows();
	float getCols();*/
	Matrix& operator= (Matrix& rhs);
	Matrix& plus (Matrix& rhs, Matrix& rhs2);
	Matrix& minus (Matrix& rhs, Matrix& rhs2);
	Matrix& transpose(Matrix& rhs);
	Matrix operator* (Matrix& rhs);

	/*Matrix& operator+ (float& );
	Matrix& operator- (float& );
	Matrix& operator* (float& );
	Matrix& operator/ (float& );

	Vector2 operator* (const Vector2&);*/
	float& operator()(const unsigned& i, const unsigned j);
	void printMatrix();

	unsigned getRows();
	unsigned getCols();
	void setRows(unsigned _rows);
	void setCols(unsigned _cols);

	
};

