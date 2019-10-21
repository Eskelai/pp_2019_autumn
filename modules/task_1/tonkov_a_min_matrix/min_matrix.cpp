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

int getMinMatrix(const std::vector<int> matrix, int rows, int colums) {
	int size, rank;
	int result = 0;
	int min;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int sizem = rows * colums;
	const int count = sizem / size; // Подсчитываем процессы
	const int rest = sizem % size; //	Учитываем остатки

	if (sizem < size) {
		if (rank == 0) {
			result = *std::min_element(matrix.begin(), matrix.end()); // Работаем с остатками
			return result;
		}
		else {
			return result;
		}
	}

	std::vector<int> buffer(count);

	if (rank == 0) {
		for (int process = 1; process < size; process++) {
			MPI_Send(&matrix[0] + (process * count) + rest, count, MPI_INT, process, 0, MPI_COMM_WORLD); // Отправляем по процам
		}
		std::vector<int> temp_matrix = std::vector<int>(matrix.begin(), matrix.begin() + count + rest);
		min = *std::min_element(temp_matrix.begin(), temp_matrix.end());
	}
	else {
		MPI_Status Status;
		MPI_Recv(&buffer[0], count, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
		min = *std::min_element(buffer.begin(), buffer.end());
	}

	MPI_Reduce(&min, &result, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	return result;
}