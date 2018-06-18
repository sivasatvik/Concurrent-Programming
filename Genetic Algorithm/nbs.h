#ifndef NBS
#define NBS

#include <bits/stdc++.h>
#include <vector>
#include <chrono>


typedef std::pair<std::vector<int >, int > my_pair;


struct Node{
	my_pair item;
	int key;
	bool marked;
	Node *next;
};


class NBS{
public:
	NBS();
	bool add(my_pair item);
	bool contains(my_pair item);
	std::vector<int> choose(int x);
	int get_size();
	Node * get_head();

private:
	Node *head;
	Node *tail;
	int size;
};

#endif