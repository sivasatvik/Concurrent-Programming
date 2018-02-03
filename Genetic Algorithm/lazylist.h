#ifndef LZ_L
#define LZ_L

#include <vector>
#include <mutex>
#include <chrono>

std::mutex pred_mx[POP], curr_mx[POP], mx[POP];
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
	LazyList();
	bool add(my_pair item);
	bool remove(my_pair item);
	bool contains(my_pair item);
private:
	Node *head;
	Node *tail;
	bool validate(Node pred, Node curr);
};
