#include <iostream>

// Dimensione del blocco (può essere regolata per ottimizzare le prestazioni)
#define BLOCK_SIZE 4

// Funzione per ordinare un blocco con Insertion Sort
void insertionSort(int* arr, int left, int right) {
  for (int i = left + 1; i <= right; i++) {
    int key = arr[i];
    int j = i - 1;

    // Sposta gli elementi più grandi di key a destra
    while (j >= left && arr[j] > key) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

// Funzione per unire due blocchi ordinati (merge simile a Merge Sort)
void mergeBlocks(int* arr, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  int* leftBlock = new int[n1];
  int* rightBlock = new int[n2];

  // Copia i dati nei blocchi temporanei
  for (int i = 0; i < n1; i++) leftBlock[i] = arr[left + i];
  for (int i = 0; i < n2; i++) rightBlock[i] = arr[mid + 1 + i];

  int i = 0, j = 0, k = left;

  // Merge dei due blocchi
  while (i < n1 && j < n2) {
    if (leftBlock[i] <= rightBlock[j]) {
      arr[k++] = leftBlock[i++];
    }
    else {
      arr[k++] = rightBlock[j++];
    }
  }

  // Copia gli elementi rimanenti
  while (i < n1) arr[k++] = leftBlock[i++];
  while (j < n2) arr[k++] = rightBlock[j++];
}

int min(int x, int y) {
  return (x < y) ? x : y;
}
// Funzione principale del Block Sort
void blockSort(int* arr, int n) {

  // 1. Dividere l'array in blocchi e ordinare ciascun blocco con Insertion Sort
  for (int i = 0; i < n; i += BLOCK_SIZE) {
    int end = min(i + BLOCK_SIZE - 1, n - 1);
    insertionSort(arr, i, end);
  }

  // 2. Unire i blocchi ordinati progressivamente
  for (int size = BLOCK_SIZE; size < n; size *= 2) {
    for (int left = 0; left < n; left += 2 * size) {
      int mid = left + size - 1;
      int right = min(left + 2 * size - 1, n - 1);
      if (mid < right) {
        mergeBlocks(arr, left, mid, right);
      }
    }
  }
}

// Funzione per stampare l'array
void printArray(const int* arr, int n) {
  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;
}

// Funzione principale
int main() {
  int arr[] = { 29, 10, 14, 37, 13, 12, 6, 42, 19, 17 };

  std::cout << "Array prima dell'ordinamento: ";
  printArray(arr, 10);

  blockSort(arr, 10);

  std::cout << "Array dopo Block Sort: ";
  printArray(arr, 10);

  return 0;
}
