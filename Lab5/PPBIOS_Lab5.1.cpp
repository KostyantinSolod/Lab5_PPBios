#include <iostream>
#include <mpi.h>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);  // Ініціалізація MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Отримуємо ранг процесу
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Отримуємо кількість процесів

    vector<int> matrix;
    int totalSum = 0;
    int localSum = 0;
    int N = 50000;

    if (rank == 0) {
        // Ініціалізуємо масив на процесі 0
        matrix.resize(N);
        for (int i = 0; i < N; ++i) {
            matrix[i] = i + 1;  // Заповнюємо масив числами від 1 до N
        }
    }

    // Розподіляємо частини масиву серед процесів
    int chunkSize = N / size;
    vector<int> localArray(chunkSize);
    MPI_Scatter(matrix.data(), chunkSize, MPI_INT, localArray.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Обчислюємо локальну суму на кожному процесі
    for (int i = 0; i < chunkSize; ++i) {
        localSum += localArray[i];
    }

    // Збираємо всі локальні суми в загальну суму
    MPI_Reduce(&localSum, &totalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Процес 0 виводить результат
    if (rank == 0) {
        cout << "Загальна сума елементів масиву: " << totalSum << endl;
    }

    MPI_Finalize();  // Завершення роботи з MPI
    return 0;
}
