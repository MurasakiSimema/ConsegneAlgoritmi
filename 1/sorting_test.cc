#include <iostream>

int read_count = 0;

void insertion_sort(int* a, int n) {
  int i, j, key;
  for (i = 1; i < n; i++) {
    key = a[i];
    read_count++;
    j = i - 1;
    while (j >= 0 && a[j] > key) {
      read_count++;
      a[j + 1] = a[j];
      read_count += 2;
      j = j - 1;
    }
    read_count++;
    a[j + 1] = key;
    read_count++;
  }
}

void reverse_insertion_sort(int* a, int n) {
  int i, j, key;
  for (i = n - 2; i >= 0; i--) {
    key = a[i];
    read_count++;
    j = i + 1;
    while (j < n && a[j] > key) {
      read_count++;
      a[j - 1] = a[j];
      read_count += 2;
      j = j + 1;
    }
    read_count++;
    a[j - 1] = key;
    read_count++;
  }

  for (int i = 0; i < n / 2;++i) {
    int tmp = a[i];
    read_count++;
    a[i] = a[n - i - 1];
    read_count++;
    a[n - i - 1] = tmp;
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
  int maxI = i;

  for (i = 0; i < r - q; ++i) {
    R[i] = A[q + 1 + i];
    ++read_count;
  }
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

void swap(int& a, int& b) {
  int tmp = a;
  a = b;
  b = tmp;
}

void heapify(int arr[], int n, int i)
{
  int largest = i; // Initialize largest as root Since we are using 0 based indexing
  int l = 2 * i + 1; // left = 2*i + 1
  int r = 2 * i + 2; // right = 2*i + 2

  // If left child is larger than root

  int arrLargest = arr[largest];
  ++read_count;
  if (l < n && arr[l] > arrLargest) {
    ++read_count;
    largest = l;
    arrLargest = arr[l];
    ++read_count;
  }
  ++read_count;

  // If right child is larger than largest so far
  if (r < n && arr[r] > arrLargest) {
    ++read_count;
    largest = r;
    arrLargest = arr[r];
    ++read_count;
  }
  ++read_count;

  // If largest is not root
  if (largest != i) {
    swap(arr[i], arr[largest]);
    read_count += 2;

    // Recursively heapify the affected sub-tree
    heapify(arr, n, largest);
  }
}

// main function to do heap sort
void heapSort(int arr[], int n)
{
  // Build heap (rearrange array)
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);

  // One by one extract an element from heap
  for (int i = n - 1; i >= 0; i--) {
    // Move current root to end
    swap(arr[0], arr[i]);
    read_count += 2;

    // call max heapify on the reduced heap
    heapify(arr, i, 0);
  }
}

int min(int a, int b) {
  return a < b ? a : b;
}

// Timsort algorithm
void timSort(int arr[], int n) {
  int RUN = 32;
  // Step 1: Sort small chunks using Insertion Sort
  for (int i = 0; i < n; i += RUN)
    insertion_sort(arr + i, min((i + RUN), (n)));

  // Step 2: Merge sorted chunks
  for (int size = RUN; size < n; size = 2 * size) {
    for (int left = 0; left < n; left += 2 * size) {
      int mid = left + size - 1;
      int right = min((left + 2 * size - 1), (n - 1));

      if (mid < right) merge(arr, left, mid, right);
    }
  }
}

int shellSort(int arr[], int n)
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
      for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
        read_count++;
        arr[j] = arr[j - gap];
        read_count++;
      }
      read_count++;

      //  put temp (the original a[i]) in its correct location
      arr[j] = temp;
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

  for (int i = 0; i < n / 2; ++i) {
    int tmp = arr[i];
    read_count++;
    arr[i] = arr[n - i - 1];
    read_count++;
    arr[n - i - 1] = tmp;
  }

  return 0;
}

//int min(int x, int y) {
//  return (x < y) ? x : y;
//}
// Funzione principale del Block Sort
void blockSort(int* arr, int n) {
  const int BLOCK_SIZE = 32;
  // 1. Dividere l'array in blocchi e ordinare ciascun blocco con Insertion Sort
  for (int i = 0; i < n; i += BLOCK_SIZE) {
    int end = min(i + BLOCK_SIZE, n);
    insertion_sort(arr + i, end);
  }

  // 2. Unire i blocchi ordinati progressivamente
  for (int size = BLOCK_SIZE; size < n; size *= 2) {
    for (int left = 0; left < n; left += 2 * size) {
      int mid = left + size - 1;
      int right = min(left + 2 * size - 1, n - 1);
      if (mid < right) {
        merge(arr, left, mid, right);
      }
    }
  }
}

void reverseBlockSort(int* arr, int n){
  const int BLOCK_SIZE = 32;
  // 1. Dividere l'array in blocchi e ordinare ciascun blocco con Insertion Sort
  for (int i = 0; i < n; i += BLOCK_SIZE) {
    int end = min(i + BLOCK_SIZE, n);
    reverse_insertion_sort(arr + i, end);
  }

  // 2. Unire i blocchi ordinati progressivamente
  for (int size = BLOCK_SIZE; size < n; size *= 2) {
    for (int left = 0; left < n; left += 2 * size) {
      int mid = left + size - 1;
      int right = min(left + 2 * size - 1, n - 1);
      if (mid < right) {
        merge(arr, left, mid, right);
      }
    }
  }
}


int main() {
  int old = read_count;

  int a1[] = { 1,2,3,4,5,6,7,8,9,10 };
  int a2[] = { 10,9,8,7,6,5,4,3,2,1 };

  for (int i = 0; i < 10;++i) {
    std::cout << a1[i] << " ";
  }
  std::cout << std::endl;
  insertion_sort(a1, 10);
  std::cout << "Insertion with ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a1[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a2[i] << " ";
  }
  std::cout << std::endl;
  insertion_sort(a2, 10);
  std::cout << "Insertion with reversed ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a2[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  int a3[] = { 1,2,3,4,5,6,7,8,9,10 };
  int a4[] = { 10,9,8,7,6,5,4,3,2,1 };

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a3[i] << " ";
  }
  std::cout << std::endl;
  reverse_insertion_sort(a3, 10);
  std::cout << "Reverse Insertion with ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a3[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a4[i] << " ";
  }
  std::cout << std::endl;
  reverse_insertion_sort(a4, 10);
  std::cout << "Reverse Insertion with reversed ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a4[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  int a5[] = { 1,2,3,4,5,6,7,8,9,10 };
  int a6[] = { 10,9,8,7,6,5,4,3,2,1 };

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a5[i] << " ";
  }
  std::cout << std::endl;
  heapSort(a5, 10);
  std::cout << "Heap with ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a5[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a6[i] << " ";
  }
  std::cout << std::endl;
  heapSort(a6, 10);
  std::cout << "Heap with reversed ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a6[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  int a7[] = { 1,2,3,4,5,6,7,8,9,10 };
  int a8[] = { 10,9,8,7,6,5,4,3,2,1 };

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a7[i] << " ";
  }
  std::cout << std::endl;
  timSort(a7, 10);
  std::cout << "Tim with ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a7[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a8[i] << " ";
  }
  std::cout << std::endl;
  timSort(a8, 10);
  std::cout << "Tim with reversed ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a8[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  int a9[] = { 1,2,3,4,5,6,7,8,9,10 };
  int a10[] = { 10,9,8,7,6,5,4,3,2,1 };

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a9[i] << " ";
  }
  std::cout << std::endl;
  shellSort(a9, 10);
  std::cout << "Shell with ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a9[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a10[i] << " ";
  }
  std::cout << std::endl;
  shellSort(a10, 10);
  std::cout << "Shell with reversed ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a10[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  int a11[] = { 1,2,3,4,5,6,7,8,9,10 };
  int a12[] = { 10,9,8,7,6,5,4,3,2,1 };

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a11[i] << " ";
  }
  std::cout << std::endl;
  reverseShellSort(a11, 10);
  std::cout << "Reverse Shell with ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a11[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a12[i] << " ";
  }
  std::cout << std::endl;
  reverseShellSort(a12, 10);
  std::cout << "Reverse Shell with reversed ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a12[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  int a13[] = { 1,2,3,4,5,6,7,8,9,10 };
  int a14[] = { 10,9,8,7,6,5,4,3,2,1 };

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a13[i] << " ";
  }
  std::cout << std::endl;
  blockSort(a13, 10);
  std::cout << "Block with ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a13[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a14[i] << " ";
  }
  std::cout << std::endl;
  blockSort(a14, 10);
  std::cout << "Block with reversed ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a14[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

  int a15[] = { 1,2,3,4,5,6,7,8,9,10 };
  int a16[] = { 10,9,8,7,6,5,4,3,2,1 };

  std::cout << std::endl;
  for (int i = 0; i < 10;++i) {
    std::cout << a15[i] << " ";
  }
  std::cout << std::endl;
  reverseBlockSort(a15, 10);
  std::cout << "Reverse Block with ordered: " << (read_count - old) << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << a15[i] << " ";
  }
  std::cout << std::endl;
  old = read_count;

}