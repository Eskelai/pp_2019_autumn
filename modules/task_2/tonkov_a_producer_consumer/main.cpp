// Copyright 2019 Tonkov Alexei
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_2/test/producer_consumer.h"

TEST(Producer_Consumer_MPI, test_func_rootsFromNumbers) {
	int numbers[4] = {4, 16, 9, 36};
	double roots[4];
	double real_roots[4] = {2, 4, 3, 6};
	rootsFromNumbers(numbers, roots, 4);
	bool flag = true;

	for (int i = 0; i < 4; i++) {
		if (roots[i] != real_roots[i]) {
			flag = false;
			break;
		}
	}
	EXPECT_TRUE(flag);
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
