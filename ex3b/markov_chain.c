#include "markov_chain.h"
#include "string.h"
/***************************/
/*        Declarations          */
/***************************/

Node *get_num_node (MarkovChain *markov_chain, int num);
int get_random_number (int max_number);
MarkovNode *
get_node_by_frequency (const NextNodeCounter *counter_list, int i);
int safe_realloc (MarkovNode *markov_node);
bool insert_counter_list_empty (MarkovNode *first_node, MarkovNode
*second_node);
int check_in_counter_list (MarkovNode *first_node, MarkovNode *second_node,
                           MarkovChain *markov_chain);
Node *insert_node (MarkovChain *markov_chain, void *data_ptr, bool flag);
MarkovNode *create_markov_node (void *data, MarkovChain *markov_chain);
void initialize_markov_node (MarkovNode *markov_node);
int get_num_of_occur(MarkovNode *state_struct_ptr);
/***************************/

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  int rand_num = get_random_number (markov_chain->database->size);
  Node *node = get_num_node (markov_chain, rand_num);
  while (markov_chain->is_last (node->data->data))
  {
    rand_num = get_random_number (markov_chain->database->size);
    node = get_num_node (markov_chain, rand_num);
  }
  return node->data;
}

MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  int rand_num = get_random_number (get_num_of_occur (state_struct_ptr));
  return get_node_by_frequency (state_struct_ptr->counter_list, rand_num);
}

void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  MarkovNode *next_node = NULL;
  if (first_node != NULL)
  {
    next_node = first_node;
  }
  else
  {
    next_node = get_first_random_node (markov_chain);
  }
  markov_chain->print_func (next_node->data);
  for (int i = 1; i < max_length; i++)
  {
    next_node = get_next_random_node (next_node);
    if (markov_chain->is_last (next_node->data) || i == (max_length - 1) ||
        next_node->counter_list == NULL)
    {
      markov_chain->print_func (next_node->data);
      return;
    }
    markov_chain->print_func (next_node->data);
  }
}

void free_markov_chain (MarkovChain **markov_chain)
{
  Node *curr_node = (*markov_chain)->database->first;
  Node *next_node = curr_node->next;
  for (int i = 0; i < (*markov_chain)->database->size; i++)
  {
    free (curr_node->data->counter_list);
    curr_node->data->counter_list = NULL;
    (*markov_chain)->free_data ((curr_node->data->data));
    curr_node->data->data = NULL;
    free (curr_node->data);
    curr_node->data = NULL;
    free (curr_node);
    curr_node = next_node;
    if (i == (*markov_chain)->database->size - 1)
    {
      next_node = NULL;
      continue;
    }
    next_node = next_node->next;
  }
  free((*markov_chain)->database);
  free (*markov_chain);
  *markov_chain = NULL;
}

bool add_node_to_counter_list (MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain)
{
  if (markov_chain->is_last (first_node->data))
  {
    return true;
  }
  if (first_node->counter_list == NULL) // counter_list is empty
  {
    return insert_counter_list_empty (first_node, second_node);
  }
  int counter = check_in_counter_list (first_node, second_node, markov_chain);
  if (counter != -1) // if inside counter_list already
  {
    first_node->counter_list[counter].frequency += 1;
    return true;
  }
  if (safe_realloc (first_node)) // increasing counter list size
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return false;
  }
  first_node->counter_list_size += 1;
  first_node->counter_list[first_node->counter_list_size - 1].markov_node =
      second_node;
  first_node->counter_list[first_node->counter_list_size - 1].frequency = 1;
  return true; // makefile sure to enter the right cell in counter list.
}

Node *get_node_from_database (MarkovChain *markov_chain, void *data_ptr)
{
  if (markov_chain == NULL)
  {
    return NULL;
  }
  Node *search_node = markov_chain->database->first;
  for (int i = 0; i < markov_chain->database->size; i++)
  {
    if (!markov_chain->comp_func (search_node->data->data, data_ptr)) // if equal
    {
      return search_node;
    }
    search_node = search_node->next;
  }
  return NULL;
}

Node *add_to_database (MarkovChain *markov_chain, void *data_ptr)
{
  if (markov_chain->database->first == NULL)  // first insertion to chain
  {
    return insert_node (markov_chain, data_ptr, false);
  }
  Node *search_node = get_node_from_database (markov_chain, data_ptr);
  if (search_node != NULL) // if data_ptr is inside the chain
  {
    return search_node;
  }
  return insert_node (markov_chain, data_ptr, true);  // insert to end of chain
}

Node *get_num_node (MarkovChain *markov_chain, int num)
{
  Node *num_node = markov_chain->database->first;
  if (num == 0)
  {
    return num_node;
  }
  if (num == markov_chain->database->size - 1)
  {
    return markov_chain->database->last;
  }
  while (num > 0)
  {
    num_node = num_node->next;
    num--;
  }
  return num_node;
}

int get_random_number (int max_number)
{
  return rand () % max_number;
}

MarkovNode *
get_node_by_frequency (const NextNodeCounter *counter_list, int i)
{
//  if ( !counter_list)
//  {
//    return NULL;
//  }
  int counter = 0;
  while (counter_list[counter].frequency <= i)
  {
    i -= counter_list[counter].frequency;
    counter++;
  }
  return counter_list[counter].markov_node;
}

int safe_realloc (MarkovNode *markov_node)
{
  NextNodeCounter *new_p = realloc (markov_node->counter_list,
                                    sizeof
                                        *(markov_node->counter_list)
                                    * (markov_node->counter_list_size
                                       + 1));
  if (!new_p)
  {
    return EXIT_FAILURE;
  }
  markov_node->counter_list = new_p;
  return EXIT_SUCCESS;
}

int get_num_of_occur(MarkovNode *state_struct_ptr)
{
  int num_of_occur = 0;
  for (int i = 0; i < state_struct_ptr->counter_list_size; i++)
  {
    num_of_occur += state_struct_ptr->counter_list[i].frequency;
  }
  return num_of_occur;
}
bool insert_counter_list_empty (MarkovNode *first_node, MarkovNode
*second_node)
{
  first_node->counter_list = malloc (sizeof *(first_node->counter_list));
  if (first_node->counter_list == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return false;  // memory allocation failed
  }
  first_node->counter_list->markov_node = second_node;
  first_node->counter_list->frequency = 1;
  first_node->counter_list_size = 1;
  return true;
}

int check_in_counter_list (MarkovNode *first_node, MarkovNode *second_node,
                           MarkovChain *markov_chain)
{
  int counter = 0;
  int freq = get_num_of_occur (first_node) - 1;
  while (markov_chain->comp_func
      (first_node->counter_list[counter]
           .markov_node->data,
       second_node->data))
  {
    freq -= first_node->counter_list[counter].frequency;
    counter++;
    if (freq <= 0 || counter == first_node->counter_list_size) // if
      // second node is not in counter list
    {
      return -1;
    }
  }
  return counter;
}

Node *insert_node (MarkovChain *markov_chain, void *data_ptr, bool flag)
{
  MarkovNode *markov_node = create_markov_node (data_ptr, markov_chain);
  if (!flag)  // first node insertion
  {
    if (markov_node == NULL || add (markov_chain->database, markov_node))
    {
      free (markov_node);
      markov_node = NULL;
      printf (ALLOCATION_ERROR_MASSAGE);
      return NULL;  // memory allocation failed
    }
    initialize_markov_node (markov_chain->database->first->data);
    return markov_chain->database->first;
  }
  if (markov_node == NULL || add (markov_chain->database, markov_node))
    // last node insertion
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    free (markov_node);
    markov_node = NULL;
    return NULL;  // memory allocation failed
  }
  initialize_markov_node (markov_chain->database->last->data);
  return markov_chain->database->last;
}

MarkovNode *create_markov_node (void *data, MarkovChain *markov_chain)
{
  MarkovNode *markov_node = malloc (sizeof *markov_node);
  if (markov_node == NULL)
  {
    return NULL;
  }
  markov_node->data = markov_chain->copy_func (data);
  if (markov_node->data == NULL)
  {
    free (markov_node);
    markov_node = NULL;
    return NULL;
  }
  return markov_node;
}

void initialize_markov_node (MarkovNode *markov_node)
{
  markov_node->counter_list = NULL;
  markov_node->counter_list_size = 0;
}