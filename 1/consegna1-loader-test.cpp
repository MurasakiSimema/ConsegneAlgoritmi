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
    }
    else {
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
    ++ct_read;
  }
  int maxI = i;

  for (i = 0; i < r - q; ++i) {
    R[i] = A[q + 1 + i];
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
    int li = 2147483647;
    if (l > 475) {
      li = L[i];
      ++ct_read;
    }
    int rj = R[j];
    int mk = M[k];
    ct_read += 2;

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
      ++ct_read;

      // shift earlier gap-sorted elements up until the correct
      // location for a[i] is found
      int j;
      int tempWithGap = A[i - gap];  //?2
      ++ct_read;
      for (j = i; j >= gap && tempWithGap > temp; j -= gap) {  //?2
        A[j] = tempWithGap;                                    //?2
        if (j >= gap + gap) {                                  //?2
          tempWithGap = A[j - gap - gap];                      //?2
          ++ct_read;
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
      ++ct_read;

      int j;
      int tempWithGap = A[i - gap];  //?3
      ++ct_read;
      for (j = i; j >= gap && tempWithGap < temp; j -= gap) {  //?3
        A[j] = tempWithGap;                                    //?3
        if (j >= gap + gap) {
          tempWithGap = A[j - gap - gap];  //?3
          ++ct_read;
        }
      }

      A[j] = temp;
    }
  }

  for (int i = 0; i < n / 2; ++i) {
    int tmp = A[i];
    ++ct_read;
    A[i] = A[n - i - 1];
    ++ct_read;
    A[n - i - 1] = tmp;
  }

  return 0;
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

  // srand((unsigned)time(NULL));
  // //creazione file input: NON USARE PIU' --> il file data.txt ufficiale è stato allegato, per permettere confronti equivalenti
  //  FILE *f = fopen("data.txt", "w+");
  //  int size=100;
  //  for (int j = 0; j < size; j++) {
  //      for (int i = 0; i < n; i++) {
  //          int v = 0;
  //         //  v=(int)(100000*exp(-(0.0+i-n/2)*(0.0+i-n/2)/n/n*64));
  //          v=(int)(5000*(1+sin(3.1415*(i/(n/2.0)))));
  //          v+=rand()%(1+(int)(n*pow(((i+0.0)/n),2)));
  //          fprintf(f, "%d,", v);
  //      }
  //      fprintf(f, "\n");
  //  }
  //  fclose(f);

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
    shellSort(A, 250);
    reverseShellSort(A + 250, 500);
    shellSort(A + 750, 250);

    tripleMerge(A, 249, 749, 999);

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
