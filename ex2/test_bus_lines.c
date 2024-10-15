#include <stdlib.h>
#include <stdio.h>
#include "test_bus_lines.h"
#include <stdbool.h>

#define TEST_1_FAIL "TEST 1 FAILED: program didn't manage to sort bus lines " \
"by distance\n"
#define TEST_1_PASS "TEST 1 PASSED: program managed to sort bus lines by " \
"distance\n"
#define TEST_3_FAIL "TEST 3 FAILED: program didn't manage to sort bus lines" \
" by duration\n"
#define TEST_3_PASS "TEST 3 PASSED: program managed to sort bus lines " \
"by duration\n"

// function uses bubble_sort and checks if it worked correctly
int is_sorted_by_distance (BusLine *start, BusLine *end)
{
  bubble_sort (start, end);
  for (int i = 0; start + i < end; i++)
  {
    if ((start + i)->distance > (start + i + 1)->distance)
    {
      printf (TEST_1_FAIL);
      return EXIT_FAILURE;
    }
  }
  printf (TEST_1_PASS);
  return EXIT_SUCCESS;
}

// function uses quick_sort and checks if it worked correctly
int is_sorted_by_duration (BusLine *start, BusLine *end)
{
  quick_sort (start, end);
  for (int i = 0; start + i < end; i++)
  {
    if ((start + i)->duration > (start + i + 1)->duration)
    {
      printf (TEST_3_FAIL);
      return EXIT_FAILURE;
    }
  }
  printf (TEST_3_PASS);
  return EXIT_SUCCESS;
}

//function checks if quick and bubble sort didn't change content of the
// pointers.
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original)
{
  for (int i = 0; (start_sorted + i) <= end_sorted; i++)
  {
    bool flag = false;
    for (int j = 0; (start_original + j) <= end_original; j++)
    {
      if ((start_sorted + i)->line_number == (start_original + j)->line_number
          && (start_sorted + i)->distance == (start_original + j)->distance
          && (start_sorted + i)->duration == (start_original + j)->duration)
      {
        flag = true;
      }
    }
    if (!flag)
    {
      return false;
    }
  }
  return true;
}
