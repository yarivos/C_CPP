#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H
// and #endif //EX2_REPO_SORTBUSLINES_H

// describes a bus_line by three parameters
typedef struct BusLine
{
    int line_number, distance, duration;
} BusLine;

// switches the values of two pointers
void swap_blocks (BusLine *sa, BusLine *sb);

// this function sorts all bus_line inputs by the distance using bubble sort
void bubble_sort (BusLine *start, BusLine *end);

// this function sorts all bus_line inputs by the duration using quick sort

void quick_sort (BusLine *start, BusLine *end);

// choose a random index and put all smaller numbers to the left and bigger
// to the right
BusLine *partition (BusLine *start, BusLine *end);
// write only between #define EX2_REPO_SORTBUSLINES_H and
// #endif //EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H
