#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define LAST_CELL 100
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define USAGE_ERROR "Usage: program receives 2 arguments.\n"
#define BASE_10 10
#define NUM_OF_ARGS 3
/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
    // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case there is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;
/***************************/
/*        Declarations          */
/***************************/
static int check_args (int argc);
static MarkovChain *initialize_chain (void);
static int compare_func (void const *pv_cell_num1, void const *pv_cell_num2);
static void *copy_func (void const *pv_cell);
static void free_data_func (void *pv_cell);
static void print_func (void const *pv_cell);
static bool is_last (void const *pv_cell);
/***************************/

/** Error handler **/
static int handle_error (char *error_msg, MarkovChain **database)
{
  printf ("%s", error_msg);
  if (database != NULL)
  {
    free_markov_chain (database);
  }
  return EXIT_FAILURE;
}

static int create_board (Cell *cells[BOARD_SIZE])
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    cells[i] = malloc (sizeof (Cell));
    if (cells[i] == NULL)
    {
      for (int j = 0; j < i; j++)
      {
        free (cells[j]);
      }
      handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
      return EXIT_FAILURE;
    }
    *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
  }

  for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
  {
    int from = transitions[i][0];
    int to = transitions[i][1];
    if (from < to)
    {
      cells[from - 1]->ladder_to = to;
    }
    else
    {
      cells[from - 1]->snake_to = to;
    }
  }
  return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database (MarkovChain *markov_chain)
{
  Cell *cells[BOARD_SIZE];
  if (create_board (cells) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  MarkovNode *from_node = NULL, *to_node = NULL;
  size_t index_to;
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    add_to_database (markov_chain, cells[i]);
  }

  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    from_node = get_node_from_database (markov_chain, cells[i])->data;

    if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
    {
      index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
      to_node = get_node_from_database (markov_chain, cells[index_to])
          ->data;
      add_node_to_counter_list (from_node, to_node, markov_chain);
    }
    else
    {
      for (int j = 1; j <= DICE_MAX; j++)
      {
        index_to = ((Cell *) (from_node->data))->number + j - 1;
        if (index_to >= BOARD_SIZE)
        {
          break;
        }
        to_node = get_node_from_database (markov_chain, cells[index_to])
            ->data;
        add_node_to_counter_list (from_node, to_node, markov_chain);
      }
    }
  }
  // free temp arr
  for (size_t i = 0; i < BOARD_SIZE; i++)
  {
    free (cells[i]);
  }
  return EXIT_SUCCESS;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main (int argc, char *argv[])
{
  if (check_args (argc))
  {
    return EXIT_FAILURE;
  }
  srand (strtol (argv[1], NULL, BASE_10));
  MarkovChain *markov_chain = initialize_chain ();
  if (markov_chain == NULL)
  {
    return EXIT_FAILURE;
  }
  if (fill_database (markov_chain))
  {
    free_markov_chain (&markov_chain);
    return EXIT_FAILURE;
  }
  for (int i = 0; i < strtol (argv[2], NULL, BASE_10); i++)
  {
    printf ("Random Walk %d: ", i + 1);
    generate_random_sequence (markov_chain,
                              markov_chain->database->first->data,
                              MAX_GENERATION_LENGTH);
    printf ("\n");
  }
  free_markov_chain (&markov_chain);
  return EXIT_SUCCESS;
}

static int check_args (int argc)
{
  if (argc == NUM_OF_ARGS)
  {
    return EXIT_SUCCESS;
  }
  printf (USAGE_ERROR);
  return EXIT_FAILURE;
}

static MarkovChain *initialize_chain (void)
{
  MarkovChain *markov_chain = malloc (sizeof *markov_chain);
  if (markov_chain == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  markov_chain->database = malloc (sizeof *(markov_chain->database));
  if (markov_chain->database == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    free (markov_chain);
    markov_chain = NULL;
    return NULL;
  }
  markov_chain->database->first = NULL;
  markov_chain->database->last = NULL;
  markov_chain->database->size = 0;
  markov_chain->comp_func = &compare_func;
  markov_chain->copy_func = &copy_func;
  markov_chain->free_data = &free_data_func;
  markov_chain->print_func = &print_func;
  markov_chain->is_last = &is_last;
  return markov_chain;
}

static int compare_func (void const *pv_cell_num1, void const *pv_cell_num2)
{
  int a = *(int *) pv_cell_num1;
  int b = *(int *) pv_cell_num2;
  if (a == b)
  {
    return 0;
  }
  return a > b ? 1 : -1;
}
static void *copy_func (void const *pv_cell)
{
  Cell * dest = malloc(sizeof (Cell));
  const char *src = pv_cell;
  memcpy (dest, src, sizeof (Cell));
  return (void *)dest;
}
static void free_data_func (void *pv_cell)
{
  Cell *to_free = (Cell *) pv_cell;
  free(to_free);
}
static void print_func (void const *pv_cell)
{
  const Cell *ptr_cell = (Cell *) pv_cell;
  if (is_last (ptr_cell))
  {
    printf ("[%d]", ptr_cell->number);
    return;
  }
  if (ptr_cell->ladder_to != -1)
  {
    printf("[%d]-ladder to %d -> ",ptr_cell->number, ptr_cell->ladder_to);
    return;
  }
  if (ptr_cell->snake_to != -1)
  {
    printf ("[%d]-snake to %d -> ",ptr_cell->number, ptr_cell->snake_to);
    return;
  }
  printf ("[%d] -> ", ptr_cell->number);
}
static bool is_last (void const *pv_cell)
{
  const Cell *ptr_cell = (Cell *) pv_cell;
  if (ptr_cell->number == LAST_CELL)
  {
    return true;
  }
  return false;
}
