#include "sort_bus_lines.h"

// this function sorts all bus_line inputs by the distance using bubble sort
void bubble_sort (BusLine *start, BusLine *end)
{
  for (int j = 0; start + j < end; j++)
  {
    for (int i = 0; start + i < end - j; i++)
    {
      if ((start + i)->distance > (start + i + 1)->distance)
      {
        swap_blocks ((start + i), (start + i + 1));
      }
    }
  }
}

// this function swaps values of two pointers
void swap_blocks (BusLine *sa, BusLine *sb)
{
  BusLine temp = *sa;
  *sa = *sb;
  *sb = temp;
}

// this function sorts all bus_line inputs by the duration using quick sort
void quick_sort (BusLine *start, BusLine *end)
{
  if (start < end)
  {
    BusLine *part_index = partition (start, end);
    quick_sort (start, part_index - 1);
    quick_sort (part_index + 1, end);
  }
}

// choose a random index and put all smaller numbers to the left and bigger
// to the right
BusLine *partition (BusLine *start, BusLine *end)
{
  BusLine *swap_index = start - 1;
  BusLine *part_index = end;
  for (BusLine *j = start; j <= end - 1; j++)
  {
    if (j->duration <= part_index->duration)
    {
      swap_index++;
      swap_blocks (swap_index, j);
    }
  }
  if (swap_index + 1 <= end)
  {
    swap_blocks (swap_index + 1, end);
  }
  return (swap_index + 1);
}