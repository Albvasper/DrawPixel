#pragma once
#include <vector>

class Mtx {

	private:
		unsigned rows;
		unsigned cols;
		std::vector<std::vector<float>> mat;

	 public:
		Mtx();
		~Mtx();
};

