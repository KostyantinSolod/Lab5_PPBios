#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
    const int N = 50000; // ����� ������
    long long* matrix = NULL; // ������������� ��� long long ��� ��������� ������������

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int elements_per_proc = N / size;
    long long local_sum = 0; // �������� ����

    if (rank == 0) {
        // ���������� ����� ����� � ��������� ������
        matrix = (long long*)malloc(N * sizeof(long long));
        for (int i = 0; i < N; i++) {
            matrix[i] = i + 1;
        }
    }

    // ����������� �������� ������ �� ���������
    long long* sub_matrix = (long long*)malloc(elements_per_proc * sizeof(long long));
    MPI_Scatter(matrix, elements_per_proc, MPI_LONG_LONG, sub_matrix, elements_per_proc, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // ����� ������ �������� ���� ����� ��������
    for (int i = 0; i < elements_per_proc; i++) {
        local_sum += sub_matrix[i];
    }

    // ������� ������� ���� � ������ 0
    long long total_sum = 0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // ������ 0 �������� ���������
    if (rank == 0) {
        printf("�������� ���� �������� ������: %lld\n", total_sum);
        free(matrix); // ��������� ���'���
    }

    free(sub_matrix); // ��������� ���'��� ��������
    MPI_Finalize();
    return 0;
}
