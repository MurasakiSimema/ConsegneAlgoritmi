#include <fstream>
#include <iostream>
#include <string>

int read_count = 0;
int shell_read_count = 0;
int reverse_shell_read_count = 0;
int merge_read_count = 0;
int gnome_read_count = 0;
int reverse_gnome_read_count = 0;
int radix_read_count = 0;

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
    ++merge_read_count;
  }
  int maxI = i;

  for (i = 0; i < r - q; ++i) {
    R[i] = A[q + 1 + i];
    ++read_count;
    ++merge_read_count;
  }
  int maxJ = i;

  i = 0;
  j = 0;

  for (k = p; k <= r && i < maxI && j < maxJ; ++k) {
    int li = L[i];
    int rj = R[j];
    read_count += 2;
    merge_read_count += 2;

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
    ++merge_read_count;
    ++i;
    ++k;
  }

  while (j < maxJ) {
    A[k] = R[j];
    ++read_count;
    ++merge_read_count;
    ++j;
    ++k;
  }

  delete[] L;
  delete[] R;
}

void insertionSort(int* A, int n) {
  int i, j, key;
  for (i = 1; i < n; ++i) {
    key = A[i];
    ++read_count;
    ++shell_read_count;
    j = i - 1;
    while (j >= 0 && A[j] > key) {
      ++read_count;
      ++shell_read_count;
      A[j + 1] = A[j];
      ++read_count;
      ++shell_read_count;
      j = j - 1;
    }
    ++read_count;
    ++shell_read_count;
    A[j + 1] = key;
    ++read_count;
    ++shell_read_count;
  }
}

void reverseInsertionSort(int* A, int n) {
  // insertion sort but with reversed loop
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
}

void partialMerge(int* A, int* L, int* R, int ln, int rn) {
  int i = 0;
  int j = 0;
  int k = 0;

  int n = ln + rn;

  for (k = 0; k <= n && i < ln && j < rn; ++k) {
    int li = L[i];
    int rj = R[j];
    read_count += 2;
    merge_read_count += 2;

    if (li <= rj) {
      A[k] = li;
      ++i;
    }
    else {
      A[k] = rj;
      ++j;
    }
  }

  while (i < ln) {
    A[k] = L[i];
    ++read_count;
    ++merge_read_count;
    ++i;
    ++k;
  }

  while (j < rn) {
    A[k] = R[j];
    ++read_count;
    ++merge_read_count;
    ++j;
    ++k;
  }
}

void tripleMergeWithReverseCenter(int* A, int q, int r, int s, int limit) {
  int* L = new int[q];
  int* R = new int[r - q];
  int* M = new int[s - r];

  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;

  for (i = 0; i < q + 1; ++i) {
    L[i] = A[i];
    ++read_count;
    ++merge_read_count;
  }
  int maxI = i;

  for (i = 0; i < r - q; ++i) {
    R[i] = A[r - i];
    ++read_count;
    ++merge_read_count;
  }
  int maxJ = i;

  for (i = 0; i < s - r; ++i) {
    M[i] = A[r + 1 + i];
    ++read_count;
    ++merge_read_count;
  }
  int maxK = i;

  i = 0;
  j = 0;
  k = 0;

  for (l = 0; l <= s && i < maxI && j < maxJ && k < maxK; ++l) {
    int li = 2147483647;  // MAX_INT
    if (l > limit) {
      li = L[i];
      ++read_count;
      ++merge_read_count;
    }
    int rj = R[j];
    int mk = M[k];
    read_count += 2;
    merge_read_count += 2;

    if (mk <= li && mk <= rj) {
      A[l] = mk;
      ++k;
    }
    else if (rj <= li && rj <= mk) {
      A[l] = rj;
      ++j;
    }
    else {
      A[l] = li;
      ++i;
    }
  }

  if (i >= maxI)  // L is empty
    partialMerge(A + l, R + j, M + k, maxJ - j, maxK - k);
  else if (j >= maxJ)  // R is empty
    partialMerge(A + l, L + i, M + k, maxI - i, maxK - k);
  else  // M is empty
    partialMerge(A + l, L + i, R + j, maxI - i, maxJ - j);

  delete[] L;
  delete[] R;
  delete[] M;
}

void gnomeSort(int* A, int n) {
  int index = 1;
  int arrIndex = A[1];
  int arrIndex1 = A[0];
  read_count += 2;
  gnome_read_count += 2;
  while (index < n) {
    if (arrIndex >= arrIndex1) {
      ++index;
      arrIndex1 = arrIndex;
      arrIndex = A[index];
      ++read_count;
      ++gnome_read_count;
    }
    else {
      A[index] = arrIndex1;
      A[index - 1] = arrIndex;
      index--;
      if (index == 0) {
        ++index;
        int tmp = arrIndex;
        arrIndex = arrIndex1;
        arrIndex1 = tmp;
      }
      else {
        arrIndex1 = A[index - 1];
        ++read_count;
        ++gnome_read_count;
      }
    }
  }
}

void reverseGnomeSort(int* A, int n) {
  int index = n - 2;
  int arrIndex = A[n - 2];
  int arrIndex1 = A[n - 1];
  read_count += 2;
  reverse_gnome_read_count += 2;
  while (index >= 0) {
    if (arrIndex >= arrIndex1) {
      --index;
      arrIndex1 = arrIndex;
      arrIndex = A[index];
      ++read_count;
      ++reverse_gnome_read_count;
    }
    else {
      A[index] = arrIndex1;
      A[index + 1] = arrIndex;
      index++;
      if (index == n - 1) {
        --index;
        int tmp = arrIndex;
        arrIndex = arrIndex1;
        arrIndex1 = tmp;
      }
      else {
        arrIndex1 = A[index + 1];
        ++read_count;
        ++reverse_gnome_read_count;
      }
    }
  }
}

void reverseShellSort(int* arr, int n) {
  for (int gap = n / 2; gap > 0; gap /= 15) {
    for (int i = gap; i < n; i += 1) {
      int temp = arr[i];
      ++read_count;
      ++reverse_shell_read_count;

      int j;
      int tempWithGap = arr[i - gap];
      ++read_count;
      ++reverse_shell_read_count;
      for (j = i; j >= gap && tempWithGap < temp; j -= gap) {
        arr[j] = tempWithGap;
        if (j >= gap + gap) {
          tempWithGap = arr[j - gap - gap];
          ++read_count;
          ++reverse_shell_read_count;
        }
      }

      arr[j] = temp;
    }
  }
}

void shellSort(int* arr, int n) {
  for (int gap = n / 2; gap > 0; gap /= 10) {
    for (int i = gap; i < n; i += 1) {
      int temp = arr[i];
      ++read_count;
      ++shell_read_count;

      int j;
      int tempWithGap = arr[i - gap];
      ++read_count;
      ++shell_read_count;
      for (j = i; j >= gap && tempWithGap > temp; j -= gap) {
        arr[j] = tempWithGap;
        if (j >= gap + gap) {
          tempWithGap = arr[j - gap - gap];
          ++read_count;
          ++shell_read_count;
        }
      }

      arr[j] = temp;
    }
  }
}

void countSort(int*& arr, int n, int exp, int offset = 0) {
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

  for (i = 0; i < n; i++) {
    int arrI = arr[i] - offset;
    ++read_count;
    ++radix_read_count;

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

  for (i = n - 1; i >= 0; i--) {
    int arrI = arr[i] - offset;
    ++read_count;
    ++radix_read_count;

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

  delete[] arr;
  arr = output;
}

// The main function to that sorts arr[]
// of size n using Radix Sort
void radixSort(int* arr, const int n) {
  int* output = new int[n];
  for (int i = 0; i < n; ++i) {
    output[i] = arr[i];
    ++read_count;
    ++radix_read_count;
  }

  for (int exp = 1; 1000 / exp > 0; exp *= 10) {
    countSort(output, n, exp, 0);
  }

  for (int i = 0; i < n; ++i) {
    arr[i] = output[i];
    ++read_count;
    ++radix_read_count;
  }
}

void sinusoidSort(int* A, const int firstStart, const int firstEnd, const int secondN, const int thirdN, const int mergeLimit) {
  int n = firstEnd - firstStart;
  int* arr = A + firstStart;
  gnomeSort(arr, n);

  n = secondN;
  arr = A + firstEnd;
  //reverseShellSort(arr, n);
  reverseGnomeSort(arr, n);

  n = thirdN;
  arr = A + firstEnd + secondN;
  shellSort(arr, n);
  //radixSort(arr, n);

  tripleMergeWithReverseCenter(A, firstEnd - 1, firstEnd + secondN - 1, firstEnd + secondN + thirdN - 1, mergeLimit);
}

void newSinusoidSort(int*& A) {
  const int n = 1000;
  int* arr = new int[n];

  for (int i = 0; i < n / 4;++i) {
    arr[n / 2 + i * 2] = A[i];
    ++read_count;
  }
  for (int i = 0; i < n / 4;++i) {
    arr[n / 2 + i * 2 + 1] = A[n / 4 + i];
    ++read_count;
  }
  for (int i = 0; i < n / 4;++i) {
    arr[i * 2] = A[(n / 4) * 3 - i];
    ++read_count;
  }
  for (int i = 0; i < n / 4;++i) {
    arr[i * 2 + 1] = A[(n / 4) * 3 + i + 1];
    ++read_count;
  }
  //1000

  for (int gap = n / 2; gap > 0; gap /= 22) {
    for (int i = gap; i < n; i += 1) {
      int temp = arr[i];
      ++read_count;

      int j;
      int tempWithGap = arr[i - gap];
      ++read_count;
      for (j = i; j >= gap && tempWithGap > temp; j -= gap) {
        arr[j] = tempWithGap;
        if (j >= gap + gap) {
          tempWithGap = arr[j - gap - gap];
          ++read_count;
        }
      }

      arr[j] = temp;
    }
  }

  delete[] A;
  A = arr;
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

  int merge_read_min = -1;
  int merge_read_max = -1;
  long merge_read_avg = 0;

  int shell_read_min = -1;
  int shell_read_max = -1;
  long shell_read_avg = 0;

  int reverse_shell_read_min = -1;
  int reverse_shell_read_max = -1;
  long reverse_shell_read_avg = 0;

  int gnome_read_min = -1;
  int gnome_read_max = -1;
  long gnome_read_avg = 0;

  int reverse_gnome_read_min = -1;
  int reverse_gnome_read_max = -1;
  long reverse_gnome_read_avg = 0;

  int radix_read_min = -1;
  int radix_read_max = -1;
  long radix_read_avg = 0;

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
    gnome_read_count = 0;
    reverse_gnome_read_count = 0;
    shell_read_count = 0;
    reverse_shell_read_count = 0;
    radix_read_count = 0;
    merge_read_count = 0;

    // insertionSort(A, 250);
    // std::cout << "Shell sort 1: " << read_count << " " << isOrdered(A, 250) << std::endl;

    // reverseShellSort(A + 250, 500);
    // std::cout << "Reverse shell sort 2: " << read_count << " " << isOrdered(A + 250, 500) << std::endl;

    // merge(A, 0, 249, 749);
    // std::cout << "Merge 1+2: " << read_count << std::endl;

    // shellSort(A + 750, 250);
    // std::cout << "Shell sort 3: " << read_count << " " << isOrdered(A + 750, 250) << std::endl;

    // merge(A, 0, 749, 999);
    // std::cout << "Merge (1+2)+3: " << read_count << std::endl;

    // tripleMergeWithReverseCenter(A, 249, 749, 999);
    // std::cout << "Merge 1+2+3: " << read_count << " " << isOrdered(A, 1000) << std::endl;

    // shellSort(A, 1000);

    sinusoidSort(A, 142, 252, 498, 250, 475);
    //newSinusoidSort(A);

    read_avg += read_count;
    if (read_min < 0 || read_min > read_count) read_min = read_count;
    if (read_max < 0 || read_max < read_count) read_max = read_count;

    gnome_read_avg += gnome_read_count;
    if (gnome_read_min < 0 || gnome_read_min > gnome_read_count)
      gnome_read_min = gnome_read_count;
    if (gnome_read_max < 0 || gnome_read_max < gnome_read_count)
      gnome_read_max = gnome_read_count;

    reverse_gnome_read_avg += reverse_gnome_read_count;
    if (reverse_gnome_read_min < 0 || reverse_gnome_read_min > reverse_gnome_read_count)
      reverse_gnome_read_min = reverse_gnome_read_count;
    if (reverse_gnome_read_max < 0 || reverse_gnome_read_max < reverse_gnome_read_count)
      reverse_gnome_read_max = reverse_gnome_read_count;

    reverse_shell_read_avg += reverse_shell_read_count;
    if (reverse_shell_read_min < 0 || reverse_shell_read_min > reverse_shell_read_count)
      reverse_shell_read_min = reverse_shell_read_count;
    if (reverse_shell_read_max < 0 || reverse_shell_read_max < reverse_shell_read_count)
      reverse_shell_read_max = reverse_shell_read_count;

    radix_read_avg += radix_read_count;
    if (radix_read_min < 0 || radix_read_min > radix_read_count)
      radix_read_min = radix_read_count;
    if (radix_read_max < 0 || radix_read_max < radix_read_count)
      radix_read_max = radix_read_count;

    shell_read_avg += shell_read_count;
    if (shell_read_min < 0 || shell_read_min > shell_read_count)
      shell_read_min = shell_read_count;
    if (shell_read_max < 0 || shell_read_max < shell_read_count)
      shell_read_max = shell_read_count;

    merge_read_avg += merge_read_count;
    if (merge_read_min < 0 || merge_read_min > merge_read_count)
      merge_read_min = merge_read_count;
    if (merge_read_max < 0 || merge_read_max < merge_read_count)
      merge_read_max = merge_read_count;

    areOrdered[test] = isOrdered(A, 1000);

    // std::cout << std::endl;
  }
  read_avg /= 100;
  gnome_read_avg /= 100;
  reverse_gnome_read_avg /= 100;
  reverse_shell_read_avg /= 100;
  radix_read_avg /= 100;
  shell_read_avg /= 100;
  merge_read_avg /= 100;

  std::cout << std::endl;
  std::cout << std::endl;
  for (int i = 0; i < 100; ++i)
    if (areOrdered[i] != true)
      std::cout << "Test n" << i << " is not ordered" << std::endl;

  std::cout << "N test: 100" << std::endl;
  std::cout << "First 1000 element" << std::endl;
  std::cout << "Min: " << read_min << ", Med: " << read_avg << ", Max: " << read_max << std::endl;

  std::cout << std::endl;
  std::cout << "Gnome sort read count" << std::endl;
  std::cout << "Min: " << gnome_read_min << ", Med: " << gnome_read_avg << ", Max: " << gnome_read_max << std::endl;

  std::cout << std::endl;
  std::cout << "Reverse gnome sort read count" << std::endl;
  std::cout << "Min: " << reverse_gnome_read_min << ", Med: " << reverse_gnome_read_avg << ", Max: " << reverse_gnome_read_max << std::endl;

  //std::cout << std::endl;
  //std::cout << "Reverse shell sort read count" << std::endl;
  //std::cout << "Min: " << reverse_shell_read_min << ", Med: " << reverse_shell_read_avg << ", Max: " << reverse_shell_read_max << std::endl;

  //std::cout << std::endl;
  //std::cout << "Radix sort read count" << std::endl;
  //std::cout << "Min: " << radix_read_min << ", Med: " << radix_read_avg << ", Max: " << radix_read_max << std::endl;

  std::cout << std::endl;
  std::cout << "Shell sort read count" << std::endl;
  std::cout << "Min: " << shell_read_min << ", Med: " << shell_read_avg << ", Max: " << shell_read_max << std::endl;

  std::cout << std::endl;
  std::cout << "Merge read count" << std::endl;
  std::cout << "Min: " << merge_read_min << ", Med: " << merge_read_avg << ", Max: " << merge_read_max << std::endl;
}
