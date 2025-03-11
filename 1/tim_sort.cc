#include <fstream>
#include <iostream>
#include <string>

#define RUN 251 // Size of small chunks to use Insertion Sort

int read_count = 0;


bool isOrdered(int* A, int n) {
  for (int i = 0; i < n - 1; ++i) {
    if (A[i] > A[i + 1]) return false;
  }

  return true;
}

// Insertion Sort for small subarrays (O(n²) worst-case)
void insertionSort(int* A, int n) {
  int i, j, key;
  for (i = 1; i < n; ++i) {
    key = A[i];
    ++read_count;
    j = i - 1;
    while (j >= 0 && A[j] > key) {
      ++read_count;
      A[j + 1] = A[j];
      ++read_count;
      j = j - 1;
    }
    ++read_count;
    A[j + 1] = key;
    ++read_count;
  }
}

// Merge function (like in Merge Sort)
void merge(int* A, int p, int q, int r) {
  int* L = new int[r - p + 1];
  int* R = new int[r - p + 1];

  int i = 0;
  int j = 0;
  int k = 0;

  for (i = 0; i < q - p + 1; ++i) {
    L[i] = A[p + i];
    ++read_count;
  }
  int maxI = i;

  for (i = 0; i < r - q; ++i) {
    R[i] = A[q + 1 + i];
    ++read_count;
  }
  int maxJ = i;

  i = 0;
  j = 0;

  for (k = p; k <= r && i < maxI && j < maxJ; ++k) {
    int li = L[i];
    int rj = R[j];
    read_count += 2;

    if (li <= rj) {
      A[k] = li;
      ++i;
    }
    else {
      A[k] = rj;
      ++j;
    }
  }

  while (i < maxI) {
    A[k] = L[i];
    ++read_count;
    ++i;
    ++k;
  }

  while (j < maxJ) {
    A[k] = R[j];
    ++read_count;
    ++j;
    ++k;
  }
}

int min(int a, int b) {
    return a < b ? a : b;
}
// Iterative Timsort function
void timSort(int arr[], int n) {
    // Step 1: Sort small chunks with Insertion Sort
    for (int i = 0; i < n; i += RUN)
        insertionSort(arr + i, min(i + RUN - 1, n - 1));

    // Step 2: Merge adjacent runs iteratively
    for (int size = RUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = min(left + 2 * size - 1, n - 1);

            if (mid < right) // Only merge if valid
                merge(arr, left, mid, right);
        }
    }
}

// Function to generate random data
void generateData(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;  // Random numbers from 0 to 9999
}

// Function to print array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

// Main function to test Timsort
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

  int merge_read_min = -1;
  int merge_read_max = -1;
  long merge_read_avg = 0;

  int shell_read_min = -1;
  int shell_read_max = -1;
  long shell_read_avg = 0;

  int reverse_shell_read_min = -1;
  int reverse_shell_read_max = -1;
  long reverse_shell_read_avg = 0;

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
    
    timSort(A, 1000);

    read_avg += read_count;
    if (read_min < 0 || read_min > read_count) read_min = read_count;
    if (read_max < 0 || read_max < read_count) read_max = read_count;

    areOrdered[test] = isOrdered(A, 1000);

    //std::cout << std::endl;
  }
  read_avg /= 100;
  shell_read_avg /= 100;
  reverse_shell_read_avg /= 100;
  merge_read_avg /= 100;

  std::cout << std::endl;
  std::cout << std::endl;
  for (int i = 0; i < 100; ++i)
    if (areOrdered[i] != true)
      std::cout << "Test n" << i << " is not ordered" << std::endl;

  std::cout << "N test: 100" << std::endl;
  std::cout << "First 1000 element" << std::endl;
  std::cout << "Min: " << read_min << ", Med: " << read_avg << ", Max: " << read_max << std::endl;
}
