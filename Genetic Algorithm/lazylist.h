#ifndef LZ_L
#define LZ_L


#include <bits/stdc++.h>
#include <vector>
#include <mutex>
#include <chrono>


typedef std::pair<std::vector<int >, int > my_pair;



// int hashCode(const char *str) {
//     int hash = 0;

//     for (int i = 0; i < strlen(str); i++) {
//         hash = 31 * hash + str[i];
//     }

//     return hash;
// }

struct Node{
	my_pair item;
	int key;
	bool marked;
	Node *next;
};



class LazyList{
public:
	LazyList();
	bool add(my_pair item);
	bool remove(my_pair item);
	bool contains(my_pair item);
	std::vector<int> choose(int x);
	int get_size();
	Node * get_head();
private:
	std::mutex mx[POP+1];
	Node *head;
	Node *tail;
	bool validate(Node * pred, Node * curr);
	int size;
};

#endif
