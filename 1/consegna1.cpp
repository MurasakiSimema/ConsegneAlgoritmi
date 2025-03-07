#include <iostream>
#include <fstream>
#include <string>

int read_count = 0;

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

void reverseInsertionSort(int* A, int n) {
  //insertion sort but with reversed loop
  int i, j, key;
  for (i = n - 2; i >= 0; i--) {
    key = A[i];
    ++read_count;
    j = i + 1;
    while (j < n && A[j] > key) {
      ++read_count;
      A[j - 1] = A[j];
      ++read_count;
      j = j + 1;
    }
    ++read_count;
    A[j - 1] = key;
    ++read_count;
  }

  for (int i = 0; i < n / 2;++i) {
    int tmp = A[i];
    ++read_count;
    A[i] = A[n - i - 1];
    ++read_count;
    A[n - i - 1] = tmp;
  }
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

int shellSort(int A[], int n) {
  // Start with a big gap, then reduce the gap
  for (int gap = n / 2; gap > 0; gap /= 2) {
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
      int tempWithGap = A[i - gap]; //?2
      ++read_count;
      for (j = i; j >= gap && tempWithGap > temp; j -= gap) { //?2
        A[j] = tempWithGap; //?2
        if(j >= gap + gap) { //?2
          tempWithGap = A[j - gap - gap]; //?2
          ++read_count;
        }
      }

      //  put temp (the original a[i]) in its correct location
      A[j] = temp;
    }
  }
  return 0;
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

  for (test = 0; test < 100; test++) {

    /// inizializzazione array: numeri random con range dimensione array
    for (i = 0; i < n; i++) {
      char comma;
      input_data >> A[i];
      input_data >> comma;
      B[i] = A[i];
    }

    read_count = 0;
    insertionSort(A, 250);
    reverseInsertionSort(A + 250, 500);
    merge(A, 0, 249, 749);

    shellSort(A + 750, 250);
    merge(A, 0, 749, 999);

    std::cout << "Test n" << (test + 1) << ": " << read_count << std::endl;


    read_avg += read_count;
    if (read_min < 0 || read_min > read_count)
      read_min = read_count;
    if (read_max < 0 || read_max < read_count)
      read_max = read_count;
  }
  read_avg /= 100;

  std::cout << std::endl;
  std::cout << "N test: 100" << std::endl;
  std::cout << "With 1000 element" << std::endl;
  std::cout << "Min: " << read_min << ", Med: " << read_avg << ", Max: " << read_max << std::endl;
}
