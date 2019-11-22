// Copyright 2019 Tonkov Alexei
#include <mpi.h>
#include <time.h>
#include <random>
#include <iostream>
#include "../../../modules/task_2/tonkov_a_producer_consumer/producer_consumer.h"

int* generateNumbers(int size) {
    int* numbers = new int[size];

    for (int i = 0; i < size; i++) {
        numbers[i] = i;
    }
    return numbers;
}

void rootsFromNumbers(int* numbers, double* roots, int size) {
    for (int i = 0; i < size; i++) {
        roots[i] = sqrt(numbers[i]);
    }
}

void produceConsume(int* numbers, double* roots, int MAX_NUMBERS) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int number_amount;

    if (rank == 0) {  // Producer
        number_amount = MAX_NUMBERS;
        // printf("[RANK:0] Sending %i numbers in total.\n", number_amount);

        for (int nextnum = 1 ; nextnum <= number_amount; ++nextnum) {
            // Wait for a worker to become available
            MPI_Status status;
            double root = 0;  // Temp result answer
            MPI_Recv(&root, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
                MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            // If a root was computed
            if (status.MPI_TAG > 0) {
                roots[status.MPI_TAG - 1] = root;
            }

            // printf("[RANK:0] Sending %i to RANK: %i, from index %i.\n", numbers[nextnum - 1], status.MPI_SOURCE,
            //                                                            nextnum);
            MPI_Send(&(numbers[nextnum - 1]), 1, MPI_INT, status.MPI_SOURCE,
                nextnum, MPI_COMM_WORLD);  // Sends nextnum as a tag (MPI_TAG = Number of job)
        }

        // Send termination signal to each rank when they submit their last job
        int num_terminated = 0;
        for (num_terminated = 0; num_terminated < size - 1; num_terminated++) {
            // Wait for a worker to become available
            MPI_Status status;
            double root = 0;
            MPI_Recv(&root, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
                MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            // If a root was computed
            if (status.MPI_TAG > 0) {
                roots[status.MPI_TAG - 1] = root;
            }
            // printf("[RANK:0] Terminating rank %i.\n", status.MPI_SOURCE);
            // Send termination signal (tag = 0)
            MPI_Send(&num_terminated, 1, MPI_INT, status.MPI_SOURCE,
                0, MPI_COMM_WORLD);
        }
    } else {  // Consumer
     // Announce myself to producer
        double root = 0;
        MPI_Send(&root, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        bool terminated = false;

        do {
            // Wait for a job
            int num = 0;
            MPI_Status status;
            MPI_Recv(&num, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            // printf("[%i] Received job: %i [tag = %i].\n", rank, num, status.MPI_TAG);
            if (status.MPI_TAG == 0) {
                terminated = true;
                // printf("[%i] Terminated.\n", rank);
            } else {
                root = sqrt(num);
                // printf("[%i] Submitting result: sqrt(%i) = %f.\n", rank, num, root);
                MPI_Send(&root, 1, MPI_DOUBLE, 0, status.MPI_TAG, MPI_COMM_WORLD);
            }
        } while (!terminated);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        // printf("\n\nResults:\n");
        for (int i = 0; i < number_amount; i++) {
            // printf("sqrt(%i) = %f\n", numbers[i], roots[i]);
        }
    }
}
