// Copyright 2019 Tonkov Alexei
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "./min_matrix.h"

TEST(Matrix_Min_MPI, Test_Min_Rnd_Matrix) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	std::vector<int> matrix;
	const int rows = 10;
	const int columns = 10;

	if (rank == 0) {
		matrix = getMatrixRandom(rows, columns);
	}

	int min;
	min = getMinMatrix(matrix, rows, columns);

	if (rank == 0) {
		int answer = *std::min_element(matrix.begin(), matrix.end());
		ASSERT_EQ(min, answer);
	}
}

TEST(Matrix_Min_MPI, Test_With_Cicle_Matrix) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	std::vector<int> matrix;
	const int rows = 100;
	const int columns = 100;

	if (rank == 0) {
		matrix = getMatrixCicle(rows, columns);
	}

	int min_matrix = getMinMatrix(matrix, rows, columns);
	if (rank == 0) {
		ASSERT_EQ(0, min_matrix);
	}
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
