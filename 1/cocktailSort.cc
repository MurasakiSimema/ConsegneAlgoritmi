void cocktailSort(int* a, int n) {
  bool swapped = true;
  int start = 0;
  int end = n - 1;

  while (swapped) {
    // reset the swapped flag on entering
    // the loop, because it might be true from
    // a previous iteration.
    swapped = false;

    // loop from left to right same as
    // the bubble sort
    for (int i = start; i < end; ++i) {
      int ai = a[i];
      int ai1 = a[i + 1];
      read_cout += 2;
      if (ai > ai1) {
        a[i] = ai1;
        a[i + 1] = ai;
        swapped = true;
      }
    }

    // if nothing moved, then array is sorted.
    if (!swapped)
      break;

    // otherwise, reset the swapped flag so that it
    // can be used in the next stage
    swapped = false;

    // move the end point back by one, because
    // item at the end is in its rightful spot
    --end;

    // from right to left, doing the
    // same comparison as in the previous stage
    for (int i = end - 1; i >= start; --i) {
      int ai = a[i];
      int ai1 = a[i + 1];
      read_cout += 2;
      if (ai > ai1) {
        a[i] = ai1;
        a[i + 1] = ai;
        swapped = true;
      }
    }

    // increase the starting point, because
    // the last stage would have moved the next
    // smallest number to its rightful spot.
    ++start;
  }
}