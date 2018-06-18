#include <iostream>
#include <algorithm>
#include "nbs.h"

using namespace std;

NBS::NBS(){
	head = new Node;
	tail = new Node;
	head->key = INT_MIN;
	head->next = tail;
	head->marked = false;
	tail->key = INT_MAX;
	tail->marked = false;
	tail->next = NULL;
	size = 0;
}



bool NBS::add(my_pair item){
	int key = item.second;
	int i = 0;
	while(true){
		//Have to find the node
		if(curr->key == key){
			return false;
		}
		else{
			Node * node = new Node;
			node->item = item;
			node->key = key;
			//node->next = 
			if(pred->next.compareAndSet(curr,node,false,false)){
				return true;
			}
		}
	}
}