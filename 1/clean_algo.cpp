#include <iostream>
#include <fstream>
#include <string>


int read_count = 0;

bool isOrdered(int* A, int n) {
  for (int i = 0; i < n - 1;++i) {
    if (A[i] > A[i + 1])
      return false;
  }

  return true;
}

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
  L[i] = INT_MAX; /// max int
  int maxI = i;


  for (i = 0; i < r - q; ++i) {
    R[i] = A[q + 1 + i];
    ++read_count;
  }
  R[i] = INT_MAX; /// max int
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

int shellSort(int A[], int n)
{
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
      for (j = i; j >= gap && A[j - gap] > temp; j -= gap) {
        ++read_count;
        A[j] = A[j - gap];
        ++read_count;
      }
      ++read_count;

      //  put temp (the original a[i]) in its correct location
      A[j] = temp;
    }
  }
  return 0;
}

int reverseShellSort(int arr[], int n)
{
  // Start with a big gap, then reduce the gap
  for (int gap = n / 2; gap > 0; gap /= 2) {
    // Do a gapped insertion sort for this gap size.
    // The first gap elements a[0..gap-1] are already in gapped order
    // keep adding one more element until the entire array is
    // gap sorted 
    for (int i = gap; i < n; i += 1) {
      // add a[i] to the elements that have been gap sorted
      // save a[i] in temp and make a hole at position i
      int temp = arr[i];
      read_count++;

      // shift earlier gap-sorted elements up until the correct 
      // location for a[i] is found
      int j;
      for (j = i; j >= gap && arr[j - gap] < temp; j -= gap) {
        read_count++;
        arr[j] = arr[j - gap];
        read_count++;
      }
      read_count++;

      //  put temp (the original a[i]) in its correct location
      arr[j] = temp;
    }
  }

  for(int i = 0; i < n / 2; ++i) {
    int tmp = arr[i];
    read_count++;
    arr[i] = arr[n - i - 1];
    read_count++;
    arr[n - i - 1] = tmp;
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
    //!insertionSort(A, 250);
    //!std::cout << "Insertion sort: " << read_count << std::endl;

    shellSort(A, 250);
    std::cout << "Shell sort -1: " << read_count << std::endl;

    //reverseInsertionSort(A + 250, 250);
    //std::cout << "Reverse insertion sort: " << read_count << std::endl;

    //merge(A, 0, 249, 499);
    //std::cout << "Merge 1: " << read_count << std::endl;

    //reverseInsertionSort(A + 500, 250);
    //std::cout << "Reverse insertion sort 2: " << read_count << std::endl;

    //!reverseInsertionSort(A + 250, 500);
    //!std::cout << "Reverse insertion sort: " << read_count << std::endl;

    reverseShellSort(A + 250, 500);
    std::cout << "Shell sort -2: " << read_count << std::endl;

    merge(A, 0, 249, 749);
    std::cout << "Merge 1: " << read_count << std::endl;

    //merge(A, 0, 499, 749);
    //std::cout << "Merge -2: " << read_count << std::endl;

    //!insertionSort(A + 750, 250);
    //!std::cout << "Insertion sort 2: " << read_count << std::endl;

    shellSort(A + 750, 250);
    std::cout << "Shell sort 1: " << read_count << std::endl;

    //!merge(A, 500, 749, 999);
    //!std::cout << "Merge 2: " << read_count << std::endl;

    //!merge(A, 0, 499, 999);
    //!std::cout << "Merge 3: " << read_count << std::endl;

    merge(A, 0, 749, 999);
    std::cout << "Merge 2: " << read_count << std::endl;


    read_avg += read_count;
    if (read_min < 0 || read_min > read_count)
      read_min = read_count;
    if (read_max < 0 || read_max < read_count)
      read_max = read_count;

    areOrdered[test] = isOrdered(A, 1000);
  }
  read_avg /= 100;

  std::cout << std::endl;
  for (int i = 0; i < 100;++i)
    if (areOrdered[i] != true)
      std::cout << "Test n" << i << " is not ordered" << std::endl;

  std::cout << "N test: 100" << std::endl;
  std::cout << "First 1000 element" << std::endl;
  std::cout << "Min: " << read_min << ", Med: " << read_avg << ", Max: " << read_max << std::endl;
}
