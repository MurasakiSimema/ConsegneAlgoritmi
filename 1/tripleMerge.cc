#include <iostream>

int read_count = 0;
int merge_read_count = 0;

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

void tripleMerge(int* A, int p, int q, int r, int s) {
  int* L = new int[r - p + 1];
  int* R = new int[r - p + 1];
  int* M = new int[s - r + 1];

  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;

  for (i = 0; i < q - p + 1; ++i) {
    L[i] = A[p + i];
    ++read_count;
    ++merge_read_count;
    std::cout << "L[" << i << "] = " << L[i] << std::endl;
  }
  int maxI = i;

  for (i = 0; i < r - q; ++i) {
    R[i] = A[q + 1 + i];
    ++read_count;
    ++merge_read_count;
    std::cout << "R[" << i << "] = " << R[i] << std::endl;
  }
  int maxJ = i;

  for (i = 0; i < s - r; ++i) {
    M[i] = A[r + 1 + i];
    ++read_count;
    ++merge_read_count;
    std::cout << "M[" << i << "] = " << M[i] << std::endl;
  }
  int maxK = i;

  i = 0;
  j = 0;
  k = 0;

  for (l = p; l <= s && i < maxI && j < maxJ && k < maxK; ++l) {
    int li = L[i];
    int rj = R[j];
    int mk = M[k];
    read_count += 3;
    merge_read_count += 3;

    std::cout << "li = " << li << " rj = " << rj << " mk = " << mk << std::endl;
    if (li <= rj && li <= mk) {
      std::cout<< "li" << std::endl;
      A[l] = li;
      ++i;
    }
    else if (rj <= li && rj <= mk) {
      std::cout<< "rj" << std::endl;
      A[l] = rj;
      ++j;
    }
    else {
      std::cout<< "mk" << std::endl;
      A[l] = mk;
      ++k;
    }
  }

  std::cout << "Inside merge" << std::endl;
  for (int i = 0;i < l;i++) {
    std::cout << A[i] << " ";
  }
  std::cout << std::endl;

  if (i >= maxI)        //L is empty
    partialMerge(A + l, R + j, M + k, maxJ - j, maxK - k);
  else if (j >= maxJ)   //R is empty
    partialMerge(A + l, L + i, M + k, maxI - i, maxK - k);
  else                  //M is empty
    partialMerge(A + l, L + i, R + j, maxI - i, maxJ - j);
}



int main() {
  int a1[] = { 1,2,3,4,5/*4*/,3,4,5,6,7,8/*10*/,1,2,3/*13*/ };

  tripleMerge(a1, 0, 4, 10, 13);

  for (int i = 0;i < 14;i++) {
    std::cout << a1[i] << " ";
  }
  std::cout << std::endl;
}