#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fstream>
#include <iostream>
using namespace std;

// compilazione: g++ consegna1-loader.c
//

// Il programma carica il file data.txt contenente 100 righe con dati da ordinare in modo crescente
// ./a.out
// In output viene mostrato il numero di accessi in read alla memoria per eseguire il sorting di ciascuna riga

// Obiettivo:
// Creare un algoritmo di sorting che minimizzi la somma del numero di accessi per ogni sorting di ciascuna riga del file

int ct_read = 0;

int max_dim = 0;
int ntests = 100;
int ndiv = 1;
int details = 0;
int graph = 0;

int n = 0;  /// dimensione dell'array

void print_array(int* A, int dim) {
  for (int j = 0; j < dim; j++) {
    printf("%d ", A[j]);
  }
  printf("\n");
}

/**
 * @brief Merges 2 sorted arrays into a single sorted array.
 *
 * @param A The array to store the merged result.
 * @param L The first sorted array.
 * @param R The second sorted array.
 * @param ln The length of the first sorted array.
 * @param rn The length of the second sorted array.
 */
void partialMerge(int* A, int* L, int* R, int ln, int rn) {
  int i = 0;
  int j = 0;
  int k = 0;

  int n = ln + rn;

  for (k = 0; k <= n && i < ln && j < rn; ++k) {
    int li = L[i];
    int rj = R[j];
    ct_read += 2;

    if (li <= rj) {
      A[k] = li;
      ++i;
    } else {
      A[k] = rj;
      ++j;
    }
  }

  while (i < ln) {
    A[k] = L[i];
    ++ct_read;
    ++i;
    ++k;
  }

  while (j < rn) {
    A[k] = R[j];
    ++ct_read;
    ++j;
    ++k;
  }
}

/**
 * @brief Merges an array sorted in 3 part into a single sorted array whit the center reversed.
 *
 * @param A The array
 * @param q The index of the end of the first part.
 * @param r The index of the end of the second part.
 * @param s The index of the end of the third part.
 * @param limit The number of elements to skip from the first part.
 */
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
    ++ct_read;
  }
  int maxI = i;

  for (i = 0; i < r - q; ++i) {
    R[i] = A[r - i];
    ++ct_read;
  }
  int maxJ = i;

  for (i = 0; i < s - r; ++i) {
    M[i] = A[r + 1 + i];
    ++ct_read;
  }
  int maxK = i;

  i = 0;
  j = 0;
  k = 0;

  for (l = 0; l <= s && i < maxI && j < maxJ && k < maxK; ++l) {
    int li = 2147483647;  // MAX_INT
    if (l > limit) {
      li = L[i];
      ++ct_read;
    }
    int rj = R[j];
    int mk = M[k];
    ct_read += 2;

    if (mk <= li && mk <= rj) {
      A[l] = mk;
      ++k;
    } else if (rj <= li && rj <= mk) {
      A[l] = rj;
      ++j;
    } else {
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

/**
 * @brief The Gnome Sort algorithm sorts.
 *
 * @param A The array to be sorted.
 * @param n The number of elements in the array.
 */
void gnomeSort(int* A, int n) {
  int index = 1;
  int arrIndex = A[1];
  int arrIndex1 = A[0];
  ct_read += 2;
  while (index < n) {
    if (arrIndex >= arrIndex1) {
      ++index;
      arrIndex1 = arrIndex;
      arrIndex = A[index];
      ++ct_read;
    } else {
      A[index] = arrIndex1;
      A[index - 1] = arrIndex;
      index--;
      if (index == 0) {
        ++index;
        int tmp = arrIndex;
        arrIndex = arrIndex1;
        arrIndex1 = tmp;
      } else {
        arrIndex1 = A[index - 1];
        ++ct_read;
      }
    }
  }
}

/**
 * @brief The reverse Gnome Sort algorithm sorts in reverse order.
 *
 * @param A The array to be sorted.
 * @param n The number of elements in the array.
 */
void reverseGnomeSort(int* A, int n) {
  int index = n - 2;
  int arrIndex = A[n - 2];
  int arrIndex1 = A[n - 1];
  ct_read += 2;
  while (index >= 0) {
    if (arrIndex >= arrIndex1) {
      --index;
      arrIndex1 = arrIndex;
      arrIndex = A[index];
      ++ct_read;
    } else {
      A[index] = arrIndex1;
      A[index + 1] = arrIndex;
      index++;
      if (index == n - 1) {
        --index;
        int tmp = arrIndex;
        arrIndex = arrIndex1;
        arrIndex1 = tmp;
      } else {
        arrIndex1 = A[index + 1];
        ++ct_read;
      }
    }
  }
}

/**
 * @brief The Shell Sort algorithm sort.
 *
 * @param arr The array to be sorted.
 * @param n The number of elements in the array.
 */
void shellSort(int* arr, int n) {
  for (int gap = n / 2; gap > 0; gap /= 10) {
    for (int i = gap; i < n; i += 1) {
      int temp = arr[i];
      ++ct_read;

      int j;
      int tempWithGap = arr[i - gap];
      ++ct_read;
      for (j = i; j >= gap && tempWithGap > temp; j -= gap) {
        arr[j] = tempWithGap;
        if (j >= gap + gap) {
          tempWithGap = arr[j - gap - gap];
          ++ct_read;
        }
      }

      arr[j] = temp;
    }
  }
}

/**
 * @brief Sorts an array in a sinusoidal pattern.
 *
 *
 * @param A The array to be sorted.
 * @param firstStart The starting index of the first part.
 * @param firstEnd The ending index of the first part.
 * @param secondN The number of elements in the second part.
 * @param thirdN The number of elements in the third part.
 * @param mergeLimit The limit for merging.
 */
void sinusoidSort(int* A, const int firstStart, const int firstEnd, const int secondN, const int thirdN, const int mergeLimit) {
  int n = firstEnd - firstStart;
  int* arr = A + firstStart;
  gnomeSort(arr, n);

  n = secondN;
  arr = A + firstEnd;
  reverseGnomeSort(arr, n);

  n = thirdN;
  arr = A + firstEnd + secondN;
  shellSort(arr, n);

  tripleMergeWithReverseCenter(A, firstEnd - 1, firstEnd + secondN - 1, firstEnd + secondN + thirdN - 1, mergeLimit);
}

int parse_cmd(int argc, char** argv) {
  /// parsing argomento
  max_dim = 1000;

  for (int i = 1; i < argc; i++) {
    if (argv[i][1] == 'd')
      ndiv = atoi(argv[i] + 3);
    if (argv[i][1] == 't')
      ntests = atoi(argv[i] + 3);
    if (argv[i][1] == 'v')
      details = 1;
    if (argv[i][1] == 'g') {
      graph = 1;
      ndiv = 1;
      ntests = 1;
    }
  }

  return 0;
}

int main(int argc, char** argv) {
  int i, test;
  int* A;
  int* B;  /// buffer per visualizzazione algoritmo

  if (parse_cmd(argc, argv))
    return 1;

  /// allocazione array
  A = new int[max_dim];

  n = max_dim;

  ifstream input_data;
  input_data.open("data.txt");

  int read_min = -1;
  int read_max = -1;
  long read_avg = 0;

  //// lancio ntests volte per coprire diversi casi di input random
  for (test = 0; test < ntests; test++) {
    /// inizializzazione array: numeri random con range dimensione array
    for (i = 0; i < n; i++) {
      char comma;
      input_data >> A[i];
      input_data >> comma;
    }

    if (details) {
      printf("caricato array di dimensione %d\n", n);
      print_array(A, n);
    }

    ct_read = 0;

    /// algoritmo di sorting
    sinusoidSort(A, 142, 252, 498, 250, 475);

    if (details) {
      printf("Output:\n");
      print_array(A, n);
    }

    /// statistiche
    read_avg += ct_read;
    if (read_min < 0 || read_min > ct_read)
      read_min = ct_read;
    if (read_max < 0 || read_max < ct_read)
      read_max = ct_read;
    printf("Test %d %d\n", test, ct_read);
  }

  printf("N test: %d, Min: %d, Med: %.1f, Max: %d\n",
         ntests,
         read_min, (0.0 + read_avg) / ntests, read_max);

  delete[] A;

  return 0;
}
