#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define USAGE_MSG "USAGE: this program accepts one of three possible \
arguments: "\
"1)test "\
"2)bubble "\
"3)quick\n"
#define BUFFER_SIZE 60
#define LINE_INFO_MSG "Enter line info. Then enter\n"
#define LINE_NUM_MSG "Enter number of lines. Then enter\n"
#define LINE_NUM_ERR_MSG "ERROR: Number of lines should be bigger than zero\n"
#define TEST_TWO_FAIL "TEST 2 FAILED: Original Bus Lines is different than \
sorted Bus Lines\n"
#define TEST_TWO_PASS "TEST 2 PASSED: Original Bus Lines is equal to sorted"\
" Bus Lines\n"
#define TEST_FOUR_FAIL "TEST 4 FAILED: Original Bus Lines is different than \
sorted Bus Lines\n"
#define TEST_FOUR_PASS "TEST 4 PASSED: Original Bus Lines is equal to sorted"\
" Bus Lines\n"
#define LINE_NUM_ERR "ERROR: Line Number should be an integer between 1 and "\
"999.\n"
#define LINE_DIST_ERR "ERROR: Distance should be an integer between 0 " \
"and 1000.\n"
#define LINE_DUR_ERR "ERROR: Duration should be an integer between " \
"10 and 100.\n"
#define NUM_BASE 10
#define MAX_LINE_NUM 999
#define MAX_DIST 1000
#define MIN_DUR 10
#define MAX_DUR 100

int check_num_args (int num_args);
int check_valid_action (const char argv[]);
int get_inputs (BusLine *ptr, int num_line);
void get_line (BusLine *ptr);
int check_line_inputs (int line_num, int dist, int dur);
void insert_line (BusLine *ptr, int line_num, int dist, int dur);
int choose_action (const char arg[], BusLine *ptr, int num_lines);
int go_test (BusLine *ptr, int num_lines);
void go_bubble (BusLine *ptr, int num_lines);
void go_quick (BusLine *ptr, int num_lines);
int get_user_input (char *buff);
int get_num_lines ();
void print_bus_lines (BusLine *ptr, size_t len);
void check_sort_dist (BusLine *ptr, int num_lines, BusLine *dup_ptr1);
void
check_sorted_dur (int num_lines, BusLine *dup_ptr1, BusLine *dup_ptr2);

/**
 * This Program accepts n buslines and sorts them in order according to
 * duration of the bus line or the distance of the bus line.
 * choose test to check if the sorts are working properly
 * choose quick to sort by duration
 * choose bubble to sort by distance
 */
int main (int argc, char *argv[])
{
  if (check_num_args (argc) || check_valid_action (argv[1]))
  {   // checking validity of arguments
    printf (USAGE_MSG);
    return EXIT_FAILURE;
  }
  int num_lines = get_num_lines ();
  BusLine *ptr = malloc (sizeof (BusLine) * num_lines);
  if (ptr == NULL)
  {
    return EXIT_FAILURE;
  }
  if (choose_action (argv[1], ptr, num_lines))
  {
    free (ptr);
    ptr = NULL;
    return EXIT_FAILURE;
  }
  free (ptr);
  ptr = NULL;
  return EXIT_SUCCESS;
}
// choose action from 3 possible inputs
int choose_action (const char arg[], BusLine *ptr, const int num_lines)
{
  get_inputs (ptr, num_lines);
  if (!strcmp (arg, "test"))
  {
    if (go_test (ptr, num_lines))
    {
      return EXIT_FAILURE;
    }
  }
  if (!strcmp (arg, "bubble"))
  {
    go_bubble (ptr, num_lines);
  }
  if (!strcmp (arg, "quick"))
  {
    go_quick (ptr, num_lines);
  }
  return EXIT_SUCCESS;
}
// test was chosen
int go_test (BusLine *ptr, const int num_lines)
{
  BusLine *dup_ptr1 = malloc (sizeof (BusLine) * num_lines);
  if (dup_ptr1 == NULL)
  {
    return EXIT_FAILURE;
  }
  BusLine *dup_ptr2 = malloc (sizeof (BusLine) * num_lines);
  if (dup_ptr2 == NULL)
  {
    free (dup_ptr1);
    dup_ptr1 = NULL;
    return EXIT_FAILURE;
  }
  memcpy (dup_ptr1, ptr, sizeof (BusLine) * (num_lines));
  memcpy (dup_ptr2, ptr, sizeof (BusLine) * (num_lines));
  check_sort_dist (ptr, num_lines, dup_ptr1);
  check_sorted_dur (num_lines, dup_ptr1, dup_ptr2);
  free (dup_ptr1);
  free (dup_ptr2);
  dup_ptr1 = NULL;
  dup_ptr2 = NULL;
  return EXIT_SUCCESS;
}
void
// run bubble quick and checked if it succeeded
check_sorted_dur (const int num_lines, BusLine *dup_ptr1, BusLine *dup_ptr2)
{
  is_sorted_by_duration (dup_ptr1, dup_ptr1 + num_lines - 1);
  if (is_equal (dup_ptr1, dup_ptr1 + num_lines - 1, dup_ptr2, dup_ptr2 +
                                                              num_lines
                                                              - 1))
  {
    printf (TEST_FOUR_PASS);
  }
  else
  {
    printf (TEST_FOUR_FAIL);
  }
}
// run bubble sort and checked if it succeeded
void check_sort_dist (BusLine *ptr, const int num_lines, BusLine *dup_ptr1)
{
  is_sorted_by_distance (ptr, ptr + num_lines - 1);
  if (is_equal (ptr, ptr + num_lines - 1, dup_ptr1, dup_ptr1 +
                                                    num_lines - 1))
  {
    printf (TEST_TWO_PASS);
  }
  else
  {
    printf (TEST_TWO_FAIL);
  }
}

// action bubble
void go_bubble (BusLine *ptr, const int num_lines)
{
  bubble_sort (ptr, ptr + num_lines - 1);
  print_bus_lines (ptr, num_lines);
}
// action quick
void go_quick (BusLine *ptr, const int num_lines)
{
  quick_sort (ptr, ptr + num_lines - 1);
  print_bus_lines (ptr, num_lines);
}

/// gets a line from user and stores values inside ptr.
/// \param ptr, buffer
void get_line (BusLine *ptr)
{
  char buffer[BUFFER_SIZE];
  bool flag = false;
  while (!flag)
  {
    printf (LINE_INFO_MSG);
    if (!get_user_input (buffer))
    {
      int line_num, dist, dur;
      sscanf (buffer, "%d,%d,%d", &line_num, &dist, &dur);
      if (check_line_inputs (line_num, dist, dur))
      {
        continue;
      }
      else
      {
        insert_line (ptr, line_num, dist, dur);
      }
      flag = true;
    }
  }
}

// get all bus_line inputs
int get_inputs (BusLine *ptr, const int num_lines)
{
  for (int i = 0; i < num_lines; i++)
  {
    get_line (ptr + i);
  }
  return EXIT_SUCCESS;
}

// gets number of line user want to input
int get_num_lines ()
{
  char buffer[BUFFER_SIZE];
  char *end_ptr = NULL;
  bool flag = false;
  int num_lines;
  while (!flag)
  {
    printf (LINE_NUM_MSG);
    if (!get_user_input (buffer))
    {
      num_lines = strtol (buffer, &end_ptr, NUM_BASE);
      if (num_lines <= 0 || buffer == end_ptr)
      {
        printf (LINE_NUM_ERR_MSG);
        continue;
      }
      flag = true;
    }
  }
  end_ptr = NULL;
  return num_lines;
}

// get user input inside buffer
int get_user_input (char *buffer)
{
  if (fgets (buffer, BUFFER_SIZE, stdin) != NULL)
  {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

// prints all values in ptr in reverse order
void print_bus_lines (BusLine *ptr, size_t len)
{
  for (long unsigned int i = 0; i < len; i++)
  {
    printf ("%d,%d,%d\n", ptr->line_number, ptr->distance, ptr->duration);
    ptr++;
  }
}

/// checks if number of arguments is equal to one
/// \param num_args
int check_num_args (int num_args)
{
  if (num_args == 2)
  {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}

/// checks if program received a valid action
/// \param argv
int check_valid_action (const char argv[])
{
  if (!strcmp (argv, "test"))
  {
    return EXIT_SUCCESS;
  }
  if (!strcmp (argv, "bubble"))
  {
    return EXIT_SUCCESS;
  }
  if (!strcmp (argv, "quick"))
  {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

// checks if line parameters are valid
int check_line_inputs (int line_num, int dist, int dur)
{
  if (line_num < 1
      || line_num > MAX_LINE_NUM)
  {

    printf (LINE_NUM_ERR);
    return EXIT_FAILURE;
  }
  if (dist < 0
      || dist > MAX_DIST)
  {
    printf (LINE_DIST_ERR);
    return EXIT_FAILURE;
  }
  if (dur < MIN_DUR
      || dur > MAX_DUR)
  {
    printf (LINE_DUR_ERR);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

//inserts a bus line to pointer type bus line
void insert_line (BusLine *ptr, int line_num, int dist, int dur)
{
  ptr->line_number = line_num;
  ptr->distance = dist;
  ptr->duration = dur;
}
