#include "pch.h"
#include "Matrix.h"
#include <iostream>

Matrix::Matrix(unsigned rows, unsigned cols){
	mat.resize(rows);
	for (unsigned i = 0; i < mat.size(); i++){
		mat[i].resize(cols, 0);
	}	
	/*rows = _rows;
	cols = _cols;*/
}

float& Matrix::operator()(const unsigned& i, const unsigned j) {
	return (mat[i][j]);
}


void Matrix::setRows(unsigned _rows) {
	rows = _rows;
}

void Matrix::setCols(unsigned _cols) {
	cols = _cols;
}

unsigned Matrix::getRows() {
	return rows;
}

unsigned Matrix::getCols() {
	return cols;
}

Matrix& Matrix::plus(Matrix& rhs, Matrix& rhs2) {
	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {
			rhs(i, j) = rhs(i, j) + rhs2(i, j);
			std::cout << rhs(i, j) << std::endl;
		}
	}
	return rhs;
}

Matrix& Matrix::minus(Matrix& rhs, Matrix& rhs2) {
	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {
			rhs(i, j) = rhs(i, j) - rhs2(i, j);
			std::cout << rhs(i, j) << std::endl;
		}
	}
	return rhs;
}

Matrix& Matrix::transpose(Matrix& rhs) {
	Matrix transposeM(getRows(), getCols());
	for (unsigned i = 0; i < rows; ++i)
		for (unsigned j = 0; j < cols; ++j) {
			transposeM(j, i) = rhs(i, j);
			std::cout << transposeM(i, j) << std::endl;
		}
	return transposeM;
}

Matrix& Matrix::operator= (Matrix& rhs) {
	return rhs;
}

void Matrix::printMatrix() {
	/*for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {
			rhs(i, j) = rhs(i, j) - mat2(i, j);
			std::cout <<  << std::endl;
		}
	}*/
}

//Matrix Matrix::operator*(Matrix& rhs) {
//	Matrix mat2(getRows(), getCols());
//	Matrix mult(getRows(), getCols());
//	for (unsigned i = 0; i < rows; i++) {
//		for (unsigned j = 0; j < cols; j++) {
//			for (unsigned k = 0; k < cols; k++) {
//				mult(i,j) += rhs(i, j) * mat2(i, j);
//			}
//		}
//	}
//	return mult;
//}

Matrix::~Matrix() {
}
