#include <fstream>
#include <iostream>

int read_count = 0;

bool isOrdered(int* A, int n) {
  for (int i = 0; i < n - 1; ++i) {
    if (A[i] > A[i + 1]) return false;
  }

  return true;
}

void merge(int* A, int p, int q, int r, int limit = 0) {
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
    int li = 2147483647;
    if (k > limit) {
      li = L[i];
      ++read_count;
    }
    int rj = R[j];
    ++read_count;

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

void countSort(int*& arr, int n, int exp, int offset = 0) {
  // Output array
  std::cout << "new int " << n << std::endl;
  int* output = new int[n];
  int i;
  int n0 = 0;
  int n1 = 0;
  int n2 = 0;
  int n3 = 0;
  int n4 = 0;
  int n5 = 0;
  int n6 = 0;
  int n7 = 0;
  int n8 = 0;
  int n9 = 0;

  for (i = 0; i < n / 2; i++) {
    int arrI = arr[i] - offset;
    read_count++;
    switch ((arrI / exp) % 10) {
      case 0:
        n0++;
        break;
      case 1:
        n1++;
        break;
      case 2:
        n2++;
        break;
      case 3:
        n3++;
        break;
      case 4:
        n4++;
        break;
      case 5:
        n5++;
        break;
      case 6:
        n6++;
        break;
      case 7:
        n7++;
        break;
      case 8:
        n8++;
        break;
      case 9:
        n9++;
        break;
    }
  }

  n1 += n0;
  n2 += n1;
  n3 += n2;
  n4 += n3;
  n5 += n4;
  n6 += n5;
  n7 += n6;
  n8 += n7;
  n9 += n8;

  for (i = n / 2 - 1; i >= 0; i--) {
    int arrI = arr[i] - offset;
    read_count++;

    switch ((arrI / exp) % 10) {
      case 0:
        output[n0 - 1] = arrI + offset;
        n0--;
        break;
      case 1:
        output[n1 - 1] = arrI + offset;
        n1--;
        break;
      case 2:
        output[n2 - 1] = arrI + offset;
        n2--;
        break;
      case 3:
        output[n3 - 1] = arrI + offset;
        n3--;
        break;
      case 4:
        output[n4 - 1] = arrI + offset;
        n4--;
        break;
      case 5:
        output[n5 - 1] = arrI + offset;
        n5--;
        break;
      case 6:
        output[n6 - 1] = arrI + offset;
        n6--;
        break;
      case 7:
        output[n7 - 1] = arrI + offset;
        n7--;
        break;
      case 8:
        output[n8 - 1] = arrI + offset;
        n8--;
        break;
      case 9:
        output[n9 - 1] = arrI + offset;
        n9--;
        break;
    }
  }

  std::cout << i << std::endl;
  std::cout << "arr2" << std::endl;
  n0 = 0;
  n1 = 0;
  n2 = 0;
  n3 = 0;
  n4 = 0;
  n5 = 0;
  n6 = 0;
  n7 = 0;
  n8 = 0;
  n9 = 0;

  for (i = n / 2; i < n; i++) {
    int arrI = arr[i];
    read_count++;
    switch ((arrI / exp) % 10) {
      case 0:
        n0++;
        break;
      case 1:
        n1++;
        break;
      case 2:
        n2++;
        break;
      case 3:
        n3++;
        break;
      case 4:
        n4++;
        break;
      case 5:
        n5++;
        break;
      case 6:
        n6++;
        break;
      case 7:
        n7++;
        break;
      case 8:
        n8++;
        break;
      case 9:
        n9++;
        break;
    }
  }

  n1 += n0;
  n2 += n1;
  n3 += n2;
  n4 += n3;
  n5 += n4;
  n6 += n5;
  n7 += n6;
  n8 += n7;
  n9 += n8;

  int* output2 = output + n / 2;
  for (i = n - 1; i >= n / 2; i--) {
    int arrI = arr[i];
    read_count++;

    switch ((arrI / exp) % 10) {
      case 0:
        output2[n0 - 1] = arrI;
        n0--;
        break;
      case 1:
        output2[n1 - 1] = arrI;
        n1--;
        break;
      case 2:
        output2[n2 - 1] = arrI;
        n2--;
        break;
      case 3:
        output2[n3 - 1] = arrI;
        n3--;
        break;
      case 4:
        output2[n4 - 1] = arrI;
        n4--;
        break;
      case 5:
        output2[n5 - 1] = arrI;
        n5--;
        break;
      case 6:
        output2[n6 - 1] = arrI;
        n6--;
        break;
      case 7:
        output2[n7 - 1] = arrI;
        n7--;
        break;
      case 8:
        output2[n8 - 1] = arrI;
        n8--;
        break;
      case 9:
        output2[n9 - 1] = arrI;
        n9--;
        break;
    }
  }

  std::cout << i << std::endl;
  delete[] arr;
  arr = output;
  std::cout << "-----------------------------------------" << std::endl;
}

// The main function to that sorts arr[]
// of size n using Radix Sort
void radixsort(int*& arr) {
  for (int exp = 1; 5000 / exp > 0; exp *= 10) {
    countSort(arr, 1000, exp, 2000);
  }

  merge(arr, 0, 499, 999, 475);
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

    radixsort(A);

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
