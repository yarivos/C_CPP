#include "markov_chain.h"
#include "string.h"
#define MAX_TWEET_CHAR 1000
#define MAX_WORD 100
#define INFINITY 4000000
#define MAX_WORDS_IN_TWEET 20
#define LIMIT_WORDS_TO_READ_ARGS 5
#define NO_LIMIT_WORDS_TO_READ_ARGS 4
#define BASE_10 10

/***************************/
/*        Declarations          */
/***************************/
static int check_file (FILE **f_ptr, char *argv);
static int check_args (int argc);
static MarkovChain *initialize_chain (void);
static int
fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain);
static int
separate_words (MarkovChain *markov_chain, char *s, int *words_to_read);
static int compare_func (void const *pv_str1, void const *pv_str2);
static void *copy_func (void const *pv_str);
static void free_data_func (void *pv_str);
static void print_func (void const *pv_str);
static bool is_last (void const *pv_str);
/***************************/

int main (int argc, char *argv[])
{
  FILE *f_ptr = NULL;
  if (check_args (argc) || check_file (&f_ptr, argv[3]))
  {
    return EXIT_FAILURE;
  }
  srand (strtol (argv[1], NULL, BASE_10));
  MarkovChain *markov_chain = initialize_chain ();
  if (markov_chain == NULL)
  {
    fclose (f_ptr);
    return EXIT_FAILURE;
  }
  int words_to_read = (argc == LIMIT_WORDS_TO_READ_ARGS) ? strtol (argv[4],
                                                                   NULL,
                                                                   BASE_10) :
                      INFINITY;
  if (fill_database (f_ptr, words_to_read, markov_chain))
  {
    free_markov_chain (&markov_chain);
    fclose (f_ptr);
    return EXIT_FAILURE;
  }
  for (int i = 0; i < strtol (argv[2], NULL, BASE_10); i++)
  {
    printf ("Tweet %d: ", i + 1);
    generate_random_sequence (markov_chain, NULL, MAX_WORDS_IN_TWEET);
    printf ("\n");
  }
  fclose (f_ptr);
  free_markov_chain (&markov_chain);
  return EXIT_SUCCESS;
}

static int check_file (FILE **f_ptr, char *argv)
{
  *f_ptr = fopen (argv, "r");
  if (*f_ptr == NULL)
  {
    printf ("Error: invalid path to file.\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
static int check_args (int argc)
{
  if (argc == NO_LIMIT_WORDS_TO_READ_ARGS || argc == LIMIT_WORDS_TO_READ_ARGS)
  {
    return EXIT_SUCCESS;
  }
  printf ("Usage: program receives 3 to 4 arguments.\n");
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
static int
fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  char buff[MAX_TWEET_CHAR];
  while (fgets (buff, MAX_TWEET_CHAR, fp) && words_to_read > 0)
  {
    if (separate_words (markov_chain, buff, &words_to_read))
    {
      return EXIT_FAILURE; // memory allocation failed
    }
  }
  return EXIT_SUCCESS;
}
static int
separate_words (MarkovChain *markov_chain, char *s, int *words_to_read)
{
  const char del[3] = " \n";
  char *token = NULL;
  token = strtok (s, del);
  if (token == NULL)
  {
    return EXIT_SUCCESS;
  }
  Node *last_node = add_to_database (markov_chain, token);
  if (last_node == NULL)
  {
    return EXIT_FAILURE;
  }
  token = strtok (NULL, del);
  (*words_to_read)--;
  while (token != NULL && *words_to_read > 0)
  {
    (*words_to_read)--;
    Node *curr_node = add_to_database (markov_chain, token);
    if (curr_node == NULL || !add_node_to_counter_list (last_node->data,
                                                        curr_node->data,
                                                        markov_chain))
    {
      return EXIT_FAILURE; // memory allocation failed
    }
    token = strtok (NULL, del);
    last_node = curr_node;
  }
  return EXIT_SUCCESS;
}

static int compare_func (void const *pv_str1,
                  void const *pv_str2)
{
  const char *p_str1 = pv_str1;
  const char *p_str2 = pv_str2;
  return strcmp (p_str2, p_str1);
}
static void *copy_func (void const *pv_str)
{
  const char *p_str = (char *) pv_str;
  char *p_str2 = malloc (strlen (p_str) + 1);
  return strcpy (p_str2, p_str);
}
static void free_data_func (void *pv_str)
{
  char *p_str = (char *) pv_str;
  free (p_str);
}

static void print_func (void const *pv_str1)
{
  const char *p_str1 = (char *) pv_str1;
  if (is_last (p_str1))
  {
    printf ("%s", p_str1);
    return;
  }
  printf ("%s ", p_str1);
}
static bool is_last (void const *pv_str)
{
  const char *p_str = (char *) pv_str;
  if (*(p_str + strlen (p_str) - 1) == '.')
  {
    return true;  // ends with dot
  }
  return false;
}