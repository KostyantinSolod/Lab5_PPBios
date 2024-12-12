#include <iostream>
#include <mpi.h>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);  // ����������� MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // �������� ���� �������
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // �������� ������� �������

    vector<int> matrix;
    int totalSum = 0;
    int localSum = 0;
    int N = 50000;

    if (rank == 0) {
        // ���������� ����� �� ������ 0
        matrix.resize(N);
        for (int i = 0; i < N; ++i) {
            matrix[i] = i + 1;  // ���������� ����� ������� �� 1 �� N
        }
    }

    // ����������� ������� ������ ����� �������
    int chunkSize = N / size;
    vector<int> localArray(chunkSize);
    MPI_Scatter(matrix.data(), chunkSize, MPI_INT, localArray.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    // ���������� �������� ���� �� ������� ������
    for (int i = 0; i < chunkSize; ++i) {
        localSum += localArray[i];
    }

    // ������� �� ������� ���� � �������� ����
    MPI_Reduce(&localSum, &totalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // ������ 0 �������� ���������
    if (rank == 0) {
        cout << "�������� ���� �������� ������: " << totalSum << endl;
    }

    MPI_Finalize();  // ���������� ������ � MPI
    return 0;
}
