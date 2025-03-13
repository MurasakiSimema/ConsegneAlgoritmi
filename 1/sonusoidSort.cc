int read_count = 0;

void partialMerge(int* A, int* L, int* R, int ln, int rn) {
  int i = 0;
  int j = 0;
  int k = 0;

  int n = ln + rn;

  for (k = 0; k <= n && i < ln && j < rn; ++k) {
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

  while (i < ln) {
    A[k] = L[i];
    ++read_count;
    ++i;
    ++k;
  }

  while (j < rn) {
    A[k] = R[j];
    ++read_count;
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
  }
  int maxI = i;

  for (i = 0; i < r - q; ++i) {
    R[i] = A[r - i];
    ++read_count;
  }
  int maxJ = i;

  for (i = 0; i < s - r; ++i) {
    M[i] = A[r + 1 + i];
    ++read_count;
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
    }
    int rj = R[j];
    int mk = M[k];
    read_count += 2;

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

void sinusoidSort(int* A, const int firstStart, const int firstEnd, const int secondN, const int thirdN, const int mergeLimit) {
  int n = firstEnd - firstStart;
  int* arr = A + firstStart;
  int index = 1;
  int arrIndex = arr[1];
  int arrIndex1 = arr[0];
  read_count += 2;
  while (index < n) {
    if (arrIndex >= arrIndex1) {
      index++;
      arrIndex1 = arrIndex;
      arrIndex = arr[index];
      ++read_count;
    }
    else {
      arr[index] = arrIndex1;
      arr[index - 1] = arrIndex;
      index--;
      if (index == 0) {
        ++index;
        int tmp = arrIndex;
        arrIndex = arrIndex1;
        arrIndex1 = tmp;
      }
      else {
        arrIndex1 = arr[index - 1];
        ++read_count;
      }
    }
  }

  n = secondN;
  arr = A + firstEnd;
  for (int gap = n / 2; gap > 0; gap /= 15) {
    for (int i = gap; i < n; i += 1) {
      int temp = arr[i];
      ++read_count;

      int j;
      int tempWithGap = arr[i - gap];
      ++read_count;
      for (j = i; j >= gap && tempWithGap < temp; j -= gap) {
        arr[j] = tempWithGap;
        if (j >= gap + gap) {
          tempWithGap = arr[j - gap - gap];
          ++read_count;
        }
      }

      arr[j] = temp;
    }
  }

  n = thirdN;
  arr = A + firstEnd + secondN;
  for (int gap = n / 2; gap > 0; gap /= 10) {
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

  tripleMergeWithReverseCenter(A, firstEnd - 1, firstEnd + secondN - 1, firstEnd + secondN + thirdN - 1, mergeLimit);
}