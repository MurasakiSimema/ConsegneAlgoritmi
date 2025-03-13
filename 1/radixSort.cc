#include <fstream>
#include <iostream>

int read_count = 0;

bool isOrdered(int* A, int n) {
  for (int i = 0; i < n - 1; ++i) {
    if (A[i] > A[i + 1]) return false;
  }

  return true;
}

int getMax(int arr[], int n) {
  int mx = arr[0];
  for (int i = 1; i < n; i++)
    if (arr[i] > mx)
      mx = arr[i];
  return mx;
}

void countSort(int*& arr, int n, int exp) {
  // Output array
  int* output = new int[n];
  int i, count[10] = {0};

  for (i = 0; i < n; i++) {
    count[(arr[i] / exp) % 10]++;
    read_count++;
  }

  for (i = 1; i < 10; i++) {
    count[i] += count[i - 1];
    read_count++;
  }

  for (i = n - 1; i >= 0; i--) {
    int arrI = arr[i];
    read_count++;
    output[count[(arrI / exp) % 10] - 1] = arrI;
    count[(arrI / exp) % 10]--;
  }

  delete[] arr;
  arr = output;
}

// The main function to that sorts arr[]
// of size n using Radix Sort
void radixsort(int*& arr, int n) {
  for (int exp = 1; 10062 / exp > 0; exp *= 10) {
    countSort(arr, n, exp);
  }
}

int main() {
  int i, test;
  int* A;
  int* B;
  int max_dim = 1000;

  A = new int[max_dim];
  B = new int[max_dim];

  int n = max_dim;

  std::ifstream input_data;
  input_data.open("data.txt");

  int read_min = -1;
  int read_max = -1;
  long read_avg = 0;

  bool areOrdered[100];

  for (test = 0; test < 100; test++) {
    /// inizializzazione array: numeri random con range dimensione array
    for (i = 0; i < n; i++) {
      char comma;
      input_data >> A[i];
      input_data >> comma;
      B[i] = A[i];
    }

    read_count = 0;

    radixsort(A, 1000);

    read_avg += read_count;
    if (read_min < 0 || read_min > read_count) read_min = read_count;
    if (read_max < 0 || read_max < read_count) read_max = read_count;

    areOrdered[test] = isOrdered(A, 1000);

    // std::cout << std::endl;
  }
  read_avg /= 100;

  std::cout << std::endl;
  std::cout << std::endl;
  for (int i = 0; i < 100; ++i)
    if (areOrdered[i] != true)
      std::cout << "Test n" << i << " is not ordered" << std::endl;

  std::cout << "N test: 100" << std::endl;
  std::cout << "First 1000 element" << std::endl;
  std::cout << "Min: " << read_min << ", Med: " << read_avg << ", Max: " << read_max << std::endl;
}
