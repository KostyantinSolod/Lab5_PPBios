#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int size, rank, total_sum = 0, local_sum = 0;
    const int N = 50000; // ����� ������
    int* matrix = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int elements_per_proc = N / size;

    // ������ 0 ������ �����
    if (rank == 0) {
        matrix = (int*)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            matrix[i] = i + 1;
        }
    }

    // ����� ������ ������ ��� �������
    int* sub_matrix = (int*)malloc(elements_per_proc * sizeof(int));
    MPI_Scatter(matrix, elements_per_proc, MPI_INT, sub_matrix, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    // ��������� ��������� ����
    for (int i = 0; i < elements_per_proc; i++) {
        local_sum += sub_matrix[i];
    }

    // ��� ��������� ��� �� ������ 0
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("�������� ���� �������� ������: %d\n", total_sum);
        free(matrix);
    }

    free(sub_matrix);
    MPI_Finalize();
    return 0;
}
