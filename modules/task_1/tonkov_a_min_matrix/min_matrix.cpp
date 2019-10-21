// Copyright 2019 Tonkov Alexei
#include <algorithm>
#include <vector>
#include <mpi.h>
#include <random>
#include <ctime>
#include "./min_matrix.h"

std::vector<int> getMatrixCicle(int rows, int columns) {
	if ((rows <= 0) || (columns <= 0)) {
		throw "ERROR: [getMatrixCicle] Zero columns or rows";
	}
	std::vector<int> matrix(rows * columns);
	for (int i = 0; i < rows * columns; i++) {
		matrix[i] = i;
	}
	return matrix;
}

std::vector<int> getMatrixRandom(int rows, int columns) {
	if ((rows <= 0) || (columns <= 0)) {
		throw "ERROR: [getMatrixRandom] Zero columns or rows";
	}
	std::vector<int> matrix(rows * columns);
	for (int i = 0; i < rows * columns; i++) {
		matrix[i] = (rand() % 100 + 1);
	}
	return matrix;
}
