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
	Matrix(Matrix& rhs);
	virtual ~Matrix();

	/*float getRows();
	float getCols();*/
	Matrix& operator= (Matrix& rhs);
	Matrix& operator+ (Matrix& rhs);
	Matrix& operator- (Matrix& rhs);
	Matrix operator* (Matrix& rhs);
	Matrix transpose();

	/*Matrix& operator+ (float& );
	Matrix& operator- (float& );
	Matrix& operator* (float& );
	Matrix& operator/ (float& );

	Vector2 operator* (const Vector2&);*/
	float& operator()(const unsigned& i, const unsigned j);

	unsigned getRows();
	unsigned getCols();
	void setRows(unsigned _rows);
	void setCols(unsigned _cols);

	
};

