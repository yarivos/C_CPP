#ifndef _MARKOV_CHAIN_H
#define _MARKOV_CHAIN_H

#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool

#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"

/**
 * insert MarkovChain struct
 */
typedef struct NextNodeCounter NextNodeCounter;
typedef struct MarkovNode MarkovNode;
typedef struct MarkovChain MarkovChain;

typedef struct MarkovNode
{
    char *data;
    NextNodeCounter *counter_list;
    int counter_list_size;
} MarkovNode;

typedef struct NextNodeCounter
{
    MarkovNode *markov_node;
    int frequency;
} NextNodeCounter;

typedef struct MarkovChain
{
    LinkedList *markov_chain;
} MarkovChain;

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode *get_first_random_node (MarkovChain *markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence must have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain (MarkovChain **ptr_chain);

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool
add_node_to_counter_list (MarkovNode *first_node, MarkovNode *second_node);

/**
* Check if data_ptr is in database. If so, return the markov_node wrapping
 * it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr);

/**
* If data_ptr in markov_chain, return it's markov_node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node *add_to_database (MarkovChain *markov_chain, char *data_ptr);

/// checks if word ends with '.'
/// \param markov_node
/// \return
bool check_end_word (MarkovNode *markov_node);

/// get node number num from markov_chain
/// \param markov_chain
/// \return
Node *get_num_node (MarkovChain *markov_chain, int num);

/// get the random node needed by its frequency
/// \param counter_list
/// \param i
/// \param total
/// \return
MarkovNode *
get_node_by_frequency (const NextNodeCounter *counter_list, int i);

/// checks if second node is already inside first node counter list
/// \param first_node
/// \param second_node
/// \return
int check_in_counter_list (MarkovNode *first_node, MarkovNode *second_node);

/// safely reallocate markov_node
/// \param old_p
/// \param new_size
/// \return
int safe_realloc (MarkovNode *old_p);

/// insertion of a node to markov_chain, insert to first or last depends on
// flag
/// \param markov_chain
/// \param data_ptr
/// \param flag
/// \return
Node *insert_node (MarkovChain *markov_chain, char *data_ptr, bool flag);

/// insert second node to first node as if counter list is empty
/// \param first_node
/// \param second_node
/// \return
bool
insert_counter_list_empty (MarkovNode *first_node, MarkovNode *second_node);

/// creates a markov node
/// \param data
/// \return
MarkovNode *create_markov_node(char *data);

#endif /* markovChain_h */
