?#include <iostream>
#include <vector>
#include <omp.h>
#include <mutex>
std::mutex mu;
template <typename T>
int col_max(const std::vector<std::vector<T> > &matrix, int col, int n) {
    T max = std::abs(matrix[col][col]);
    int maxPos = col;
    for (int i = col+1; i < n; ++i) {
        T element = std::abs(matrix[i][col]);
        if (element > max) {
            max = element;
            maxPos = i;
        }
    }
    return maxPos;
}
template <typename T>
int triangulation(std::vector<std::vector<T> > &matrix, int n) {
    unsigned int swapCount = 0;
    for (int i = 0; i < n-1; ++i) {
        unsigned int imax = col_max(matrix, i, n);
        if (i != imax) {
            swap(matrix[i], matrix[imax]);
            ++swapCount;
        }
        for (int j = i + 1; j < n; ++j) {
            T mul = -matrix[j][i]/matrix[i][i];
            for (int k = i; k < n; ++k) {
                matrix[j][k] += matrix[i][k]*mul;
            }
        }
    }
    return swapCount;
}
template <typename T>
T gauss_determinant(std::vector<std::vector<T> > &matrix, int n) {
    unsigned int swapCount = triangulation(matrix, n);
    T determinanit = 1;
    if (swapCount % 2 == 1)
        determinanit = -1;
    for (int i = 0; i < n; ++i) {
        determinanit *= matrix[i][i];
    }
    return determinanit;
}

template<typename T>
std::vector<std::vector<T>> swap_column(std::vector<std::vector<T>> &vector, std::vector<T> &vector1, int i, int n) {

    std::vector<T> temp = vector[i];
    vector[i] = vector1;
    vector1 = temp;
    return vector;
}
template <typename T>
std::vector<T> cramer_solving(std::vector<std::vector<T> > &matrix,
                              std::vector<T> &free_term_column, int n) {
    T mainDet = gauss_determinant(matrix, n);
    std::cout<<"\nmain det: "<<mainDet<<"\n";
    if (std::abs(mainDet) < 0.0001){
        std::cout<<"no solution";
        throw;
    }

    std::vector<T> solution(n);
#pragma omp parallel
    {
        std::vector<std::vector<T> > private_matrix = matrix;
        std::vector<T> free_column= free_term_column;
#pragma omp for
        for (int i = 0; i < n; ++i) {
            mu.lock();
            private_matrix = swap_column(private_matrix,free_column,i,n);
            //swap_column(private_matrix,free_column,i,n);
            for (int l = 0; l < n; ++l) {
                for (int j = 0; j < n; ++j){
                    std::cout << matrix[l][j]<< "\t";
                }
                std::cout << "\n";
            }
            solution[i] = gauss_determinant(private_matrix, n) / mainDet;
            std::cout<<"x"<<i<<"="<<solution[i]<<"\n";
            //swap_column(private_matrix,free_column,i,n);
            mu.unlock();
        }
    }
    return solution;
}



int main() {
  for (int n = 3; n < 4; n += 5) {
    std::vector<std::vector<double> > matrix(n);
    for (int i = 0; i < n; ++i) {
      matrix[i].resize(n);
      for (int j = 0; j < n; ++j){
        matrix[i][j] = rand()%10;
        std::cout << matrix[i][j]<< "\t";
      }
      std::cout << "\n";
    }
    std::vector<double> column(n);
    std::cout<<"last column";
      std::cout<<"\n";
    for (int j = 0; j < n; ++j){
      column[j] = rand()%10;
      std::cout<<column[j]<<"\t";
    }
      std::cout<<"\n";
    double start_time = omp_get_wtime();
    std::vector<double> solution = cramer_solving(matrix, column, n);
    std::cout << n << " " << omp_get_wtime() - start_time << std::endl;
  }
}