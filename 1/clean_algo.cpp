#include <fstream>
#include <iostream>
#include <string>

int read_count = 0;
int shell_read_count = 0;
int reverse_shell_read_count = 0;
int merge_read_count = 0;

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

void tripleMerge(int* A, int q, int r, int s) {
  int* L = new int[800];
  int* R = new int[800];
  int* M = new int[800];

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
  std::cout << "L: " << isOrdered(L, maxI) << std::endl;

  for (i = 0; i < r - q; ++i) {
    R[i] = A[q + 1 + i];
    ++read_count;
    ++merge_read_count;
  }
  int maxJ = i;
  std::cout << "R: " << isOrdered(R, maxJ) << std::endl;

  for (i = 0; i < s - r; ++i) {
    M[i] = A[r + 1 + i];
    ++read_count;
    ++merge_read_count;
  }
  int maxK = i;
  std::cout << "M: " << isOrdered(M, maxK) << std::endl;

  i = 0;
  j = 0;
  k = 0;

  for (l = 0; l <= s && i < maxI && j < maxJ && k < maxK; ++l) {
    int li = 2147483647;  //MAX_INT
    if (l > 475) {
      li = L[i];
      ++read_count;
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

  if (i >= maxI)        //L is empty
    partialMerge(A + l, R + j, M + k, maxJ - j, maxK - k);
  else if (j >= maxJ)   //R is empty
    partialMerge(A + l, L + i, M + k, maxI - i, maxK - k);
  else                  //M is empty
    partialMerge(A + l, L + i, R + j, maxI - i, maxJ - j);

  delete[] L;
  delete[] R;
  delete[] M;
}

/*
gap = n/2   gap change to 2         Min: 6552, Med: 6636, Max: 6696
gap = n/2   gap change to 10        Min: 3531, Med: 3653, Max: 3781
gap = n/2   gap change to 5         Min: 4173, Med: 4299, Max: 4445
*/
int shellSort(int A[], int n) {
  for (int gap = n / 2; gap > 0; gap /= 10) { //?4
    for (int i = gap; i < n; i += 1) {
      int temp = A[i];
      ++read_count;
      ++shell_read_count;

      int j;
      int tempWithGap = A[i - gap];  //?2
      ++read_count;
      ++shell_read_count;
      for (j = i; j >= gap && tempWithGap > temp; j -= gap) {  //?2
        A[j] = tempWithGap;                                    //?2
        if (j >= gap + gap) {                                  //?2
          tempWithGap = A[j - gap - gap];                      //?2
          ++read_count;
          ++shell_read_count;
        }
      }

      A[j] = temp;
    }
  }
  return 0;
}

/*
gap = n/2   gap change to 2         Min: 8107, Med: 8165, Max: 8241
gap = n/2   gap change to 15        Min: 4000, Med: 4137, Max: 4291
*/
int reverseShellSort(int arr[], int n) {
  for (int gap = n / 2; gap > 0; gap /= 15) { //?4
    for (int i = gap; i < n; i += 1) {
      int temp = arr[i];
      ++read_count;
      ++reverse_shell_read_count;

      int j;
      int tempWithGap = arr[i - gap];  //?3
      ++read_count;
      ++reverse_shell_read_count;
      for (j = i; j >= gap && tempWithGap < temp; j -= gap) {  //?3
        arr[j] = tempWithGap;                                  //?3
        if (j >= gap + gap) {
          tempWithGap = arr[j - gap - gap];  //?3
          ++read_count;
          ++reverse_shell_read_count;
        }
      }

      arr[j] = temp;
    }
  }

  for (int i = 0; i < n / 2; ++i) {
    int tmp = arr[i];
    ++read_count;
    ++reverse_shell_read_count;
    arr[i] = arr[n - i - 1];
    ++read_count;
    ++reverse_shell_read_count;
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
    shell_read_count = 0;
    reverse_shell_read_count = 0;
    merge_read_count = 0;

    shellSort(A, 250);
    std::cout << "Shell sort 1: " << read_count << " " << isOrdered(A, 250) << std::endl;

    reverseShellSort(A + 250, 500);
    std::cout << "Reverse shell sort 2: " << read_count << " " << isOrdered(A + 250, 500) << std::endl;

    //merge(A, 0, 249, 749);
    //std::cout << "Merge 1+2: " << read_count << std::endl;

    shellSort(A + 750, 250);
    std::cout << "Shell sort 3: " << read_count << " " << isOrdered(A + 750, 250) << std::endl;

    //merge(A, 0, 749, 999);
    //std::cout << "Merge (1+2)+3: " << read_count << std::endl;

    tripleMerge(A, 249, 749, 999);
    std::cout << "Merge 1+2+3: " << read_count << " " << isOrdered(A, 1000) << std::endl;

    //shellSort(A, 1000);

    read_avg += read_count;
    if (read_min < 0 || read_min > read_count) read_min = read_count;
    if (read_max < 0 || read_max < read_count) read_max = read_count;

    shell_read_avg += shell_read_count;
    if (shell_read_min < 0 || shell_read_min > shell_read_count)
      shell_read_min = shell_read_count;
    if (shell_read_max < 0 || shell_read_max < shell_read_count)
      shell_read_max = shell_read_count;

    reverse_shell_read_avg += reverse_shell_read_count;
    if (reverse_shell_read_min < 0 || reverse_shell_read_min > reverse_shell_read_count)
      reverse_shell_read_min = reverse_shell_read_count;
    if (reverse_shell_read_max < 0 || reverse_shell_read_max < reverse_shell_read_count)
      reverse_shell_read_max = reverse_shell_read_count;

    merge_read_avg += merge_read_count;
    if (merge_read_min < 0 || merge_read_min > merge_read_count)
      merge_read_min = merge_read_count;
    if (merge_read_max < 0 || merge_read_max < merge_read_count)
      merge_read_max = merge_read_count;

    areOrdered[test] = isOrdered(A, 1000);

    std::cout << std::endl;
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

  std::cout << std::endl;
  std::cout << "Shell sort read count" << std::endl;
  std::cout << "Min: " << shell_read_min << ", Med: " << shell_read_avg << ", Max: " << shell_read_max << std::endl;

  std::cout << std::endl;
  std::cout << "Reverse shell sort read count" << std::endl;
  std::cout << "Min: " << reverse_shell_read_min << ", Med: " << reverse_shell_read_avg << ", Max: " << reverse_shell_read_max << std::endl;

  std::cout << std::endl;
  std::cout << "Merge read count" << std::endl;
  std::cout << "Min: " << merge_read_min << ", Med: " << merge_read_avg << ", Max: " << merge_read_max << std::endl;
}

//?  Min: 20740, Med: 20917, Max: 21116

//?2 Min: 20116, Med: 20263, Max: 20438

//?3 Min: 19507, Med: 19595, Max: 19697

//?4 Min: 12366, Med: 12583, Max: 12875

/*
First 1000 element
Min: 19507, Med: 19595, Max: 19697

Shell sort read count
Min: 6552, Med: 6636, Max: 6696

Reverse shell sort read count
Min: 8107, Med: 8165, Max: 8241

Merge read count
Min: 4775, Med: 4793, Max: 4807



*/