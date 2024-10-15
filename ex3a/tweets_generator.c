#include "markov_chain.h"
#include "linked_list.h"
#include "string.h"
#define MAX_TWEET_CHAR 1000
#define MAX_WORD 100
#define INFINITY 4000000
#define MAX_WORDS_IN_TWEET 20
#define LIMIT_WORDS_TO_READ_ARGS 5
#define NO_LIMIT_WORDS_TO_READ_ARGS 4
#define BASE_10 10

/// insert all file words into the markov_chain
/// \param fp
/// \param words_to_read
/// \param markov_chain
int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain);

int separate_words (MarkovChain *markov_chain, char
*s, int *words_to_read);

int check_args (int argc);
int check_file (FILE **f_ptr, char *argv);
MarkovChain *initialize_chain (void);

int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
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

int main (int argc, char *argv[])
{
  FILE *f_ptr = NULL;
  int words_to_read = INFINITY;
  if (check_args (argc) || check_file (&f_ptr, argv[3]))
  {
    return EXIT_FAILURE;
  }
  srand (strtol(argv[1], NULL, BASE_10));
  MarkovChain *markov_chain = initialize_chain ();
  if (markov_chain == NULL)
  {
    return EXIT_FAILURE;
  }
  if (argc == LIMIT_WORDS_TO_READ_ARGS)
  {
    words_to_read =  strtol(argv[4], NULL, BASE_10);
  }
  if (fill_database (f_ptr, words_to_read, markov_chain))
  {
    free_markov_chain (&markov_chain);
    return EXIT_FAILURE;
  }
  for (int i = 0; i < strtol(argv[2], NULL, BASE_10); i++)
  {
    printf ("Tweet %d: ", i + 1);
    generate_random_sequence (markov_chain, NULL, MAX_WORDS_IN_TWEET);
    printf ("\n");
  }
  fclose(f_ptr);
  free_markov_chain (&markov_chain);
  return EXIT_SUCCESS;
}

int
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
                                                       curr_node->data))
    {
      return EXIT_FAILURE; // memory allocation failed
    }
    token = strtok (NULL, del);
    last_node = curr_node;
  }
  return EXIT_SUCCESS;
}

MarkovChain *initialize_chain (void)
{
  MarkovChain *markov_chain = malloc (sizeof *markov_chain);
  if (markov_chain == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  markov_chain->markov_chain = malloc (sizeof *(markov_chain->markov_chain));
  if (markov_chain->markov_chain == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    free (markov_chain);
    markov_chain = NULL;
    return NULL;
  }
  markov_chain->markov_chain->first = NULL;
  markov_chain->markov_chain->last = NULL;
  markov_chain->markov_chain->size = 0;
  return markov_chain;
}

int check_file (FILE **f_ptr, char *argv)
{
  *f_ptr = fopen (argv, "r");
  if (*f_ptr == NULL)
  {
    printf ("Error: invalid path to file.\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int check_args (int argc)
{
  if (argc == NO_LIMIT_WORDS_TO_READ_ARGS || argc == LIMIT_WORDS_TO_READ_ARGS)
  {
    return EXIT_SUCCESS;
  }
  printf ("Usage: program receives 3 to 4 arguments.\n");
  return EXIT_FAILURE;
}