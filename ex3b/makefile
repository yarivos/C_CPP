CCFLAGS = -Wall -Wextra -Wvla

CC = gcc

tweets: tweets_generator.o markov_chain.o linked_list.o
	$(CC) $^ -o tweets_generator

tweets_generator.o: tweets_generator.c
	$(CC) $(CCFLAGS) -c $^

snake: snakes_and_ladders.o markov_chain.o linked_list.o
	$(CC)  $^ -o snakes_and_ladders

snakes_and_ladders.o: snakes_and_ladders.c
	$(CC)  $(CCFLAGS) -c $<

markov_chain.o: markov_chain.c markov_chain.h
	$(CC) $(CCFLAGS) -c $^

linked_list.o: linked_list.c linked_list.h
	$(CC) $(CCFLAGS) -c $^