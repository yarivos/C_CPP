#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
// write only between #define EX2_REPO_TESTBUSLINES_H and
// #endif //EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"

// function uses bubble_sort and checks if it worked correctly
int is_sorted_by_distance (BusLine *start, BusLine *end);

// function uses quick_sort and checks if it worked correctly
int is_sorted_by_duration (BusLine *start, BusLine *end);

//function checks if quick and bubble sort didn't change content of the
// pointers.
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);
// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#endif //EX2_REPO_TESTBUSLINES_H
