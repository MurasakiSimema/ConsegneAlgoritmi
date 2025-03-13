void newSinusoidSort(int*& A) {
  const int n = 1000;
  int* arr = new int[n];

  for (int i = 0; i < n / 4;++i) {
    arr[n / 2 + i * 2] = A[i];
  }
  for (int i = 0; i < n / 4;++i) {
    arr[n / 2 + i * 2 + 1] = A[n / 4 + i];
  }
  for (int i = 0; i < n / 4;++i) {
    arr[i * 2] = A[(n / 4) * 3 - i];
  }
  for (int i = 0; i < n / 4;++i) {
    arr[i * 2 + 1] = A[(n / 4) * 3 + i + 1];
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