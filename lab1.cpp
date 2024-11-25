#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

void fillMatrix(int **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

// void printMatrix(int **matrix, int n, int m) {
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < m; j++) {
//             std::cout << matrix[i][j] << "\t";
//         }
//         std::cout << std::endl;
//     }
// }

void addMatrix(int **A, int **B, int **C, int n, int m, int k, int thread_id, int threads_num) {
    for (int i = thread_id; i < n; i += threads_num) {
        for (int j = thread_id; j < m; j += threads_num) {
            C[i][j] = A[i][j] + k * B[i][j];
        }
    }
}

int main() {
    int n = 4;
    int m = 2;
    int k = 2;
    int threads_num = 20;

    int **A = new int *[n];
    int **B = new int *[n];
    int **C = new int *[n];
    for (int i = 0; i < n; i++) {
        A[i] = new int[m];
        B[i] = new int[m];
        C[i] = new int[m];
    }

    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    srand(seed);

    fillMatrix(A, n, m);
    fillMatrix(B, n, m);

    auto creation_begin = std::chrono::high_resolution_clock::now();

    std::thread threads[threads_num];
    for (int i = 0; i < threads_num; i++) {
        threads[i] = std::thread(addMatrix, A, B, C, n, m, k, i, threads_num);
    }

    for (int i = 0; i < threads_num; i++) {
        threads[i].join();
    }

    auto creation_end = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(creation_end - creation_begin);
    std::printf("Creation Time: %.5f seconds.\n", elapsed.count() * 1e-9);

    // std::cout << "Matrix A:" << std::endl;
    // printMatrix(A, n, m);
    // std::cout << "Matrix B:" << std::endl;
    // printMatrix(B, n, m);
    // std::cout << "Matrix C = A + " << k << "*B:" << std::endl;
    // printMatrix(C, n, m);

    for (int i = 0; i < n; i++) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
