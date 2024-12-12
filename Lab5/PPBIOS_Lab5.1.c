#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int size, rank, total_sum = 0, local_sum = 0;
    const int N = 50000; // Розмір масиву
    int* matrix = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int elements_per_proc = N / size;

    // Процес 0 генерує масив
    if (rank == 0) {
        matrix = (int*)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            matrix[i] = i + 1;
        }
    }

    // Кожен процес отримує свій підмасив
    int* sub_matrix = (int*)malloc(elements_per_proc * sizeof(int));
    MPI_Scatter(matrix, elements_per_proc, MPI_INT, sub_matrix, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    // Локальний підрахунок суми
    for (int i = 0; i < elements_per_proc; i++) {
        local_sum += sub_matrix[i];
    }

    // Збір локальних сум на процесі 0
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Загальна сума елементів масиву: %d\n", total_sum);
        free(matrix);
    }

    free(sub_matrix);
    MPI_Finalize();
    return 0;
}
