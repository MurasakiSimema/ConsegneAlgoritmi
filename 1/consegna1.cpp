#include <fstream>
#include <iostream>
#include <string>

int read_count = 0;

bool isOrdered(int* A, int n) {
  for (int i = 0; i < n - 1; ++i) {
    if (A[i] > A[i + 1]) return false;
  }

  return true;
}

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
    } else {
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

  delete[] L;
  delete[] R;
}

int shellSort(int A[], int n) {
  // Start with a big gap, then reduce the gap
  for (int gap = n / 2; gap > 0; gap /= 10) {  //?4
    // Do a gapped insertion sort for this gap size.
    // The first gap elements a[0..gap-1] are already in gapped order
    // keep adding one more element until the entire array is
    // gap sorted
    for (int i = gap; i < n; i += 1) {
      // add a[i] to the elements that have been gap sorted
      // save a[i] in temp and make a hole at position i
      int temp = A[i];
      ++read_count;

      // shift earlier gap-sorted elements up until the correct
      // location for a[i] is found
      int j;
      int tempWithGap = A[i - gap];  //?2
      ++read_count;
      for (j = i; j >= gap && tempWithGap > temp; j -= gap) {  //?2
        A[j] = tempWithGap;                                    //?2
        if (j >= gap + gap) {                                  //?2
          tempWithGap = A[j - gap - gap];                      //?2
          ++read_count;
        }
      }

      //  put temp (the original a[i]) in its correct location
      A[j] = temp;
    }
  }
  return 0;
}

int reverseShellSort(int A[], int n) {
  for (int gap = n / 2; gap > 0; gap /= 15) {  //?4
    for (int i = gap; i < n; i += 1) {
      int temp = A[i];
      ++read_count;

      int j;
      int tempWithGap = A[i - gap];  //?3
      ++read_count;
      for (j = i; j >= gap && tempWithGap < temp; j -= gap) {  //?3
        A[j] = tempWithGap;                                    //?3
        if (j >= gap + gap) {
          tempWithGap = A[j - gap - gap];  //?3
          ++read_count;
        }
      }

      A[j] = temp;
    }
  }

  for (int i = 0; i < n / 2; ++i) {
    int tmp = A[i];
    ++read_count;
    A[i] = A[n - i - 1];
    ++read_count;
    A[n - i - 1] = tmp;
  }

  return 0;
}

int main() {
  int i, test;
  int* A;
  int* B;
  int max_dim = 1000;
  const int testSize = 100;

  A = new int[max_dim];
  B = new int[max_dim];

  int n = max_dim;

  std::ifstream input_data;
  input_data.open("data.txt");

  int read_min = -1;
  int read_max = -1;
  long read_avg = 0;

  bool areOrdered[testSize];
  for (test = 0; test < testSize; test++) {
    /// inizializzazione array: numeri random con range dimensione array
    for (i = 0; i < n; i++) {
      char comma;
      input_data >> A[i];
      input_data >> comma;
      B[i] = A[i];
    }

    read_count = 0;
    shellSort(A, 250);
    reverseShellSort(A + 250, 500);
    merge(A, 0, 249, 749);

    shellSort(A + 750, 250);
    merge(A, 0, 749, 999);

    read_avg += read_count;
    if (read_min < 0 || read_min > read_count)
      read_min = read_count;
    if (read_max < 0 || read_max < read_count)
      read_max = read_count;

    areOrdered[test] = isOrdered(A, 1000);
  }
  read_avg /= testSize;

  for (int i = 0; i < testSize; ++i)
    if (areOrdered[i] != true)
      std::cout << "Test n" << i << " is not ordered" << std::endl;

  std::cout << std::endl;
  std::cout << "N test: " << testSize << std::endl;
  std::cout << "With " << n << " element" << std::endl;
  std::cout << "Min: " << read_min << ", Med: " << read_avg << ", Max: " << read_max << std::endl;

  delete[] A;
  delete[] B;
}
