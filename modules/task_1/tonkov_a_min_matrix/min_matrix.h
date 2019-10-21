// Copyright 2019 Tonkov Alexei
#ifndef MODULES_TASK_1_TONKOV_A_MIN_MATRIX_MIN_MATRIX_H_
#define MODULES_TASK_1_TONKOV_A_MIN_MATRIX_MIN_MATRIX_H_

#include <mpi.h>
#include <vector>

std::vector<int> getMatrixCicle(int rows, int columns);
std::vector<int> getMatrixRandom(int rows, int columns);
int getMinMatrix(const std::vector<int> matrix, int rows, int cols);

#endif  // MODULES_TASK_1_TONKOV_A_MIN_MATRIX_MIN_MATRIX_H_
