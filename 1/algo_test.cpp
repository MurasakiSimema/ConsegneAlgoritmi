#include <iostream>
#include <fstream>
#include <string>

#pragma region 

int ct_read = 0;


void swap(int& a, int& b) {
  int tmp = a;
  a = b;
  b = tmp;
  /// aggiorno contatore globale di swap
}

int partition(int* A, int p, int r) {

  /// copia valori delle due meta p..q e q+1..r
  ct_read++;
  int x = A[r];
  int i = p - 1;

  for (int j = p; j < r; j++) {
    ct_read++;
    if (A[j] <= x) {
      i++;
      ct_read++;
      ct_read++;
      swap(A[i], A[j]);
    }
  }
  ct_read++;
  ct_read++;
  swap(A[i + 1], A[r]);

  return i + 1;
}

void quick_sort(int* A, int p, int r) {
  /// gli array L e R sono utilizzati come appoggio per copiare i valori: evita le allocazioni nella fase di merge
  if (p < r) {
    int q = partition(A, p, r);
    quick_sort(A, p, q - 1);
    quick_sort(A, q + 1, r);
  }
}


#pragma endregion

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

int min(int a, int b) {
  return a < b ? a : b;
}

// Timsort algorithm
void timSort(int A[], int n) {
  int RUN = 500;
  // Step 1: Sort small chunks using Insertion Sort
  for (int i = 0; i < n; i += RUN)
    insertionSort(A + i, min((i + RUN), (n)));

  // Step 2: Merge sorted chunks
  for (int size = RUN; size < n; size = 2 * size) {
    for (int left = 0; left < n; left += 2 * size) {
      int mid = left + size - 1;
      int right = min((left + 2 * size - 1), (n - 1));

      if (mid < right) merge(A, left, mid, right);
    }
  }
}

void shellSort(int A[], int n)
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
  
}

void reverseShellSort(int arr[], int n)
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

    //timSort(A + 750, 250);
    //std::cout << "Tim sort 1: " << read_count << std::endl;

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

    //timSort(A, 1000);
    //std::cout << "Tim sort All: " << read_count << std::endl;

    //heapSort(A, 1000);
    //std::cout << "Heap sort All: " << read_count << std::endl;


    read_avg += read_count;
    if (read_min < 0 || read_min > read_count)
      read_min = read_count;
    if (read_max < 0 || read_max < read_count)
      read_max = read_count;

    areOrdered[test] = isOrdered(A, 1000);

    quick_sort(B, 0, 1000);
  }
  read_avg /= 100;

  std::cout << std::endl;
  for (int i = 0; i < 100;++i)
    if (areOrdered[i] != true)
      std::cout << "Test n" << i << " is not ordered" << std::endl;

  std::cout << "N test: 100" << std::endl;
  std::cout << "First 1000 element" << std::endl;
  std::cout << "Min: " << read_min << ", Med: " << read_avg << ", Max: " << read_max << std::endl;
  std::cout << "Quick: " << (ct_read / 100) << std::endl;
}

//Insertion sort 0..249 + Reverse insertion sort 250..499 + merge 0..249 with 250..499  ==>  10* faster quick sort 0...499

//------------------

//Insertion sort 0..249 + Reverse insertion sort 250..499 + merge 0..249 with 250..499 ==> Min: 8735, Med: 9082, Max: 9435

//Quick 0..750 ==> Med: 123395

//123395 / 9082 = 13.58

//------------------

//Insertion sort 0..249 + Reverse insertion sort 250..499 + merge 0..249 with 250..499 + Reverse insertion sort 500..749 + heap sort 750..999 + merge 0..499 with 500..999 + merge 0..999 ==> Min: 25275, Med: 25690, Max: 26068

//Quick 0..999 ==> Med: 93011

// 93011 / 25690 = 3.62

//------------------

//Insertion sort 0..249 + Reverse insertion sort 250..499 + merge 0..249 with 250..499 + Reverse insertion sort 500..749 + tim sort 750..999 + merge 0..499 with 500..999 + merge 0..999 ==> Min: 19325, Med: 19915, Max: 20642

//Quick 0..999 ==> Med: 98288

// 98288 / 20664 = 4.76

//------------------

//Insertion sort 0..249 + Reverse insertion sort 250..499 + merge 0..249 with 250..499 + Reverse insertion sort 500..749 + Insertion sort 750..999 + merge 0..499 with 500..999 + merge 0..999 ==> Min: 14930, Med: 15410, Max: 15901

//Quick 0..999 ==> Med: 93011

// 93011 / 15410 = 6.035

//------------------

//Insertion sort 0..249 + Reverse insertion sort 250..749 + merge 0..249 with 250..749 + Insertion sort 750..999 + merge 0..749 with 750..999 ==> Min: 14194, Med: 14679, Max: 15164

//Quick 0..999 ==> Med: 93011

// 93011 / 14679 = 6.336

//------------------

//Insertion sort 0..249 + Reverse insertion sort 250..749 + merge 0..249 with 250..749 + Shell sort 750..999 + merge 0..749 with 750..999 ==> Min: 14423, Med: 14789, Max: 15153

//Quick 0..999 ==> Med: 93011

// 93011 / 14789 = 6.28

//------------------

//Shell sort 0..249 + Shell sort 250..749 + merge 0..249 with 250..749 + Shell sort 750..999 + merge 0..749 with 750..999 ==> Min: 23382, Med: 23572, Max: 23710

//Quick 0..999 ==> Med: 93011

// 93011 / 23572 = 3.94

//------------------

//Shell sort 0..249 + Reverse shell sort 250..749 + merge 0..249 with 250..749 + Shell sort 750..999 + merge 0..749 with 750..999 ==> Min: 20740, Med: 20917, Max: 21116

//Quick 0..999 ==> Med: 93011

// 93011 / 20917 = 4.44

//------------------
