#ifndef LFREE_H_ 
#define LFREE_H_


#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <bits/stdc++.h>
#include <vector>
#include <algorithm> // sort, next_permutation
#include <chrono>
#include <utility> // pair
#include <iostream>

#include "atomic_ops_if.h"

#ifdef DEBUG
#define IO_FLUSH                        fflush(NULL)
#endif

typedef std::pair<std::vector<int >, int > my_pair;

typedef struct node 
{
	my_pair data;
	struct node *next;
} node_t;

typedef struct llist 
{
	node_t *head;
	node_t *tail;
	uint32_t size;
} llist_t;

inline int is_marked_ref(long i);
inline long unset_mark(long i);
inline long set_mark(long i);
inline long get_unmarked_ref(long w);
inline long get_marked_ref(long w);


llist_t* list_new();
//return 0 if not found, positive number otherwise
int list_contains(llist_t *the_list, my_pair val);
//return 0 if value already in the list, positive number otherwise
int list_add(llist_t *the_list, my_pair val);
//return 0 if value already in the list, positive number otherwise
// int list_remove(llist_t *the_list, my_pair val);
// void list_delete(llist_t *the_list);
int list_size(llist_t *the_list);


std::vector<int> choose(llist_t* the_list, int index);

node_t* new_node(my_pair val, node_t* next);
node_t* list_search(llist_t* the_list, my_pair val, node_t** left_node);

node_t* get_head(llist_t* the_list);

#endif